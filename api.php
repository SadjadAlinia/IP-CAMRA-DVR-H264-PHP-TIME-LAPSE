<?php

$main = "/home/naranjel/public_html/ipcamera";
set_time_limit(0);

require "$main/vendor/autoload.php";
$path = "$main/output";

$server = "x.x.x.x"; //192.168.1.1
$port = 80;
$username = "admin"; 
$password = "xxxxx";
$sock;
$idconnect;
$ChannelNum;

define("command_login", 0x3e8);
define("command_login_res", 0x3e9);
define("command_system_info", 0x3fc);
define("command_system_info_res", 0x3fd);
define("command_keep_alive", 0x3ee);
define("command_claim", 0x585);
define("command_claim_res", 0x586);
define("command_start", 0x582);
define("command_start_res", 0x583);
define("command_stop", 0x582);
define("command_stop_res", 0x583);
define("command_data", 0x584);
define("command_Read", 0x61a);

function hex_dump($data, $newline = "<br>")
{
    static $from = '';
    static $to = '';

    static $width = 16; # number of bytes per line

    static $pad = '.'; # padding for non-visible characters

    if ($from === '') {
        for ($i = 0; $i <= 0xFF; $i++) {
            $from .= chr($i);
            $to .= ($i >= 0x20 && $i <= 0x7E) ? chr($i) : $pad;
        }
    }

    $hex = str_split(bin2hex($data), $width * 2);
    $chars = str_split(strtr($data, $from, $to), $width);

    $offset = 0;
    foreach ($hex as $i => $line) {
        echo sprintf('%06X', $offset) . ' : ' . implode(' ', str_split($line, 2)) . ' [' . $chars[$i] . ']' . $newline;
        $offset += $width;
    }
}

function sofia_hash($msg)
{
    $md = md5($msg, true);
    $chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    $out = "";
    for ($i = 0; $i < 8; $i++) {
        $y1 = (2 * $i);
        $y2 = (2 * $i + 1);
        $n = (ord($md[$y1]) + ord($md[$y2])) % 62;
        $out .=  $chars[$n];
    }
    return $out;
}

function MakeCommad($id, $command, $data, $mode = 0)
{
    $output = pack("CCx2LLx2SL", 255, $mode, $id, 0, $command, strlen($data) + 2);
    $output .= $data . "\x0a\x00";
    return $output;
}

function ReadCommand($str, $sensId = null)
{
    if ($str == null)
        return "is null";

    $output = unpack("Chead/Cmode/x2/Lid/Lpack/x2/Scommand/Llength", $str);
    if ($output['length'] > 2) {
        $sub = substr($str, 20, $output['length'] - 2);
        $output["data"] = json_decode($sub, true);
    }
    return $output;
}

function DVR_Connct()
{
    global $server, $port, $username, $password, $sock, $idconnect, $ChannelNum;
    $sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    $sockconnect = socket_connect($sock, $server, $port);

    $maxchanel = $maxchanel = null;

    if (is_bool($sockconnect) && $sockconnect == true) {
        $pass = sofia_hash($password);
        $data = "{ \"EncryptType\" : \"MD5\", \"LoginType\" : \"DVRIP-Web\", \"PassWord\" : \"{$pass}\", \"UserName\" : \"{$username}\" }";
        socket_write($sock, MakeCommad(0, command_login, $data));
        $read = socket_read($sock, 2048);
        $rescon = ReadCommand($read);
        if ($rescon['data']['Ret'] == 100) {
            $data =  "{\"Name\": \"KeepAlive\", \"SessionID\": \"" . "0x" . dechex($idconnect) . "\"}";
            socket_write($sock, MakeCommad(0, command_keep_alive, $data));
            $read = socket_read($sock, 2048);
            $res = ReadCommand($read);
            if ($res['data']['Ret'] == 100) {
                $ChannelNum = $rescon['data']['ChannelNum'];
                $idconnect = $rescon['id'];
                return true;
            }
        }

        return false;
    }
}

function Get_datetime($value)
{
    $out = array();
    $out['second'] = $value & 0x3F;
    $out['minute'] = ($value & 0xFC0) >> 6;
    $out['hour'] = ($value & 0x1F000) >> 12;
    $out['day'] = ($value & 0x3E0000) >> 17;
    $out['month'] = ($value & 0x3C00000) >> 22;
    $out['year'] = (($value & 0xFC000000) >> 26) + 2000;
    return $out;
}

function Get_datetime_bsd($value)
{
    $second = $value & 0x3F;
    $minute = ($value & 0xFC0) >> 6;
    $hour = ($value & 0x1F000) >> 12;
    $day = ($value & 0x3E0000) >> 17;
    $month = ($value & 0x3C00000) >> 22;
    $year = (($value & 0xFC000000) >> 26) + 2000;
    // $out = $year . "\\" . $month . "\\" . $day . "-" . $hour . ":" . $minute . ":" . $second;
    // $out = $year . $month . $day . $hour . $minute . $second;
    $out = sprintf("%02d%02d%02d%02d%02d%02d", $year, $month, $day, $hour, $minute, $second);
    return $out;
}

function PackRead($len)
{
    global $sock;
    $data = "";
    $lenread = 2048;
    $index = 0;

    if ($len < $lenread)
        $lenread = $len;

    while (true) {
        $read = socket_read($sock, $lenread);
        $data .= $read;
        $index += strlen($read);
        if ($index >= $len)
            return $data;
        if (($index + $lenread) > $len)
            $lenread = $len - $index;
    }
}

function DVR_StopData($chanel = 0)
{
    global $sock, $idconnect, $path;
    $data = "{ \"Name\" : \"OPMonitor\", \"OPMonitor\" : { \"Action\" : \"Stop\", \"Parameter\" : { \"Channel\" : $chanel, \"CombinMode\" : \"NONE\", \"StreamType\" : \"Main\", \"TransMode\" : \"TCP\" } }, \"SessionID\" : \"" . "0x" . dechex($idconnect) . "\" }";
    socket_write($sock, MakeCommad($idconnect, command_stop, $data));
}

function DVR_GetData($readPack = 1, $chanel = 0, $TransMode = 'Main')
{
    global $sock, $idconnect, $path;

    if (file_exists($path) == false)
        mkdir($path);

    $fileout = fopen("$path/out$chanel.txt", "w");
    $filelog = fopen("$path/log$chanel.txt", "w");
    $file = fopen("$path/data$chanel.txt", "w");

    $data = "{ \"Name\" : \"OPMonitor\", \"OPMonitor\" : { \"Action\" : \"Claim\", \"Parameter\" : { \"Channel\" : $chanel, \"CombinMode\" : \"NONE\", \"StreamType\" : \"$TransMode\", \"TransMode\" : \"TCP\" } }, \"SessionID\" : \"" . "0x" . dechex($idconnect) . "\" }";
    socket_write($sock, MakeCommad($idconnect, command_claim, $data));
    $read = socket_read($sock, 200);
    $res = ReadCommand($read);
    if ($res["data"]['Ret'] == 100) {
        sleep(0.3);
        $data = "{ \"Name\" : \"OPMonitor\", \"OPMonitor\" : { \"Action\" : \"Start\", \"Parameter\" : { \"Channel\" : $chanel, \"CombinMode\" : \"NONE\", \"StreamType\" : \"$TransMode\", \"TransMode\" : \"TCP\" } }, \"SessionID\" : \"" . "0x" . dechex($idconnect) . "\" }";
        socket_write($sock, MakeCommad($idconnect, command_start, $data));

        $read = PackRead(20);
        $pcon = unpack("Chead/Cmode/x2/Lid/Lsequence_number/Ctotal/Ccur/Scommand/Llen_data", $read);

        if ($pcon['head'] == 255 && $pcon['mode'] == 1 && $pcon['id'] == $idconnect) {
            $buf = "";
            $length = 0;
            $read = PackRead(16);
            $pack = unpack("x2/Sdata_type/Cmedia/Cfps/Cw/Ch/Ltime/Llength", $read);
            if ($pack['data_type'] == 0xFC01) {
                $pack['w'] = $pack['w'] * 8;
                $pack['h'] = $pack['h'] * 8;
                $t = $pack['time'];
                $pack['time'] = Get_datetime($t);
                $pack['time_bsd'] = Get_datetime_bsd($t);
                fwrite($fileout, print_r($pack, true));
                $length = $pack['length'];

                $read = PackRead($pcon['len_data'] - 16);
                $buf .= $read;
                $length -= $pcon['len_data'] - 16;
                fwrite($fileout, print_r("L:" . $length . "     l:" . strlen($read) . "\r\n", true));
            } else {
                hex_dump($read);
                dir("Frame not support");
                return;
            }
            while (true) {
                $read = PackRead(20);
                $pcon = null;
                $pcon = unpack("Chead/Cmode/x2/Lid/Lsequence_number/Ctotal/Ccur/Smsgid/Llen_data", $read);
                if ($pcon['head'] == 255 && $pcon['mode'] == 1 && $pcon['id'] == $idconnect) {
                    $read = PackRead($pcon['len_data']);
                    $buf .= $read;
                    $length -= strlen($read);
                    fwrite($fileout, print_r("L:" . $length . "     l:" . strlen($read) . "\r\n", true));
                    if ($length <= 0) {
                        $readPack--;
                        $length = $pack['length'];
                        if ($readPack <= 0) {
                            fwrite($file, $buf);
                            fwrite($filelog, $pack["time_bsd"]);
                            return $buf;
                        }
                    }
                } else {
                    hex_dump($read);
                    dir("Frame Error");
                    return;
                }
            }
        } else {
            hex_dump($read);
            dir("Frame Error");
            return;
        }
    }
}

function FFMpeg_H264ToMp4($namefile = "data.txt", $chanel = 0)
{
    global $path, $main;

    if (file_exists($path) == false)
        mkdir($path);

    $name = explode(".", $namefile);
    $ffmpeg = FFMpeg\FFMpeg::create(
        array(
            'ffmpeg.binaries' => "$main/ffmpeg",
            'ffprobe.binaries' => "$main/ffprobe",
        )
    );
    $video = $ffmpeg->open("$path/$name[0]$chanel.txt");
    $video->filters()->synchronize();
    $video->save(new FFMpeg\Format\Video\X264(), "$path/$name[0]$chanel.mp4");
}

function FFMpeg_Mp4ToJpg($namefile = "data.mp4", $chanel = 0)
{
    global $path, $main;
    $name = explode(".", $namefile);
    $nameSave = $name[0];

    if (file_exists($path) == false)
        mkdir($path);

    if (file_exists("$path/log$chanel.txt") == true) {
        $nameSave = "$path/" . fread(fopen("$path/log$chanel.txt", 'r'), filesize("$path/log$chanel.txt")) . " ch$chanel.jpg";
        $nameExport = fread(fopen("$path/log$chanel.txt", 'r'), filesize("$path/log$chanel.txt")) . " ch$chanel.jpg";
        fwrite(fopen("$path/export.txt", "a"), $nameExport . "\r\n");
    }

    $ffmpeg = FFMpeg\FFMpeg::create(
        array(
            'ffmpeg.binaries' => "$main/ffmpeg",
            'ffprobe.binaries' => "$main/ffprobe",
        )
    );

    $video = $ffmpeg->open("$path/$name[0]$chanel.mp4");
    $video->filters()->synchronize();
    $video->frame(FFMpeg\Coordinate\TimeCode::fromSeconds(0))->save($nameSave);
}

for ($ch = 0; $ch < 3; $ch++) {
    if (DVR_Connct() == true) {
        echo "Connect <br>";
        DVR_GetData(2, $ch, 'Sub');
        DVR_StopData($ch);
        FFMpeg_H264ToMp4("data.txt", $ch);
        FFMpeg_Mp4ToJpg("data.mp4", $ch);
    }
    socket_close($sock);
}
