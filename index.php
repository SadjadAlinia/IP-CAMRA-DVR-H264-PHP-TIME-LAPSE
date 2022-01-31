<?php

session_start();

$mode = $_POST['mode'];
$output = "/home/naranjel/public_html/ipcamera/output";
$pass = "xxxx"; //loging for view

function update()
{
    $_SESSION["time"] = time() + 1000;
}

function ckeck()
{
    if ($_SESSION["login"] == 1) {
        if (time() < $_SESSION["time"]) {
            update();
            return true;
        }
    }

    $_SESSION["login"] = 0;
    return false;
}

if (strcmp($mode, "login") == 0) {
    $username = $_POST['username'];
    $password = $_POST['password'];

    if (strcmp($username, "admin") == 0 && strcmp($password, $pass) == 0) {
        $html = file_get_contents("data.html");
        $_SESSION["login"] = 1;
        update();
        echo json_encode(array("OK", $html));
    } else {
        echo json_encode(array("Error", "رمز نادرست است"));
    }
} else if (strcmp($mode, "gettime") == 0 && ckeck()) {
    $list = fread(fopen("$output/export.txt", "r"), filesize("$output/export.txt"));
    $list = explode("\r\n", $list);
    $chanel = $_POST['chanel'];
    $out = array();
    for ($i = count($list); $i > 0; $i--) {
        $f = $list[$i];
        $name = explode(".", $f);
        if (strcmp($name[1], "jpg") == 0) {
            $name = explode(" ", $name[0]);
            if (strcmp($name[1], $chanel) == 0) {
                array_push($out, $name[0]);
            }
        }
    }
    echo json_encode(array("settime", $out));
} else if (strcmp($mode, "getimg") == 0 && ckeck()) {
    $name = "$output/$_POST[name] $_POST[chanel].jpg";
    $data = fread(fopen($name, 'r'), filesize($name));
    echo json_encode(array("img", $_POST['id'], base64_encode($data)));
} else {
    if (ckeck()) {
        echo file_get_contents("data.html");
    } else
        echo file_get_contents("login.html");
}
