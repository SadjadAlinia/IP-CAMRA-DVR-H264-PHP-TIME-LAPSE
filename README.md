Welcome to the IP-CAMRA-DVR-H264-PHP-TIME-LAPSE wiki!

1.
config dvr connection api.php
$server = "x.x.x.x"; //192.168.1.1
$port = 80;
$username = "admin"; 
$password = "xxxxx";
we cant set ip local and global

2)
config user connection for view index.php
$pass = "xxxx"; //loging for view

3.
add cpanel Cron Jobs
*/5	*	*	*	*	php /home/naranjel/public_html/ipcamera/api.php

4.
naranjel >> change put your name home in Cron Jobs and index.php and api.php
edite adress /home/naranjel/public_html/

good luck my frind
