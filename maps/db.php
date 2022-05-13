<?php

$dbname = 'capstone' ;
$dbuser = 'root';
$dbhost = 'localhost';
$dbpassword='';

$connect = @mysqli_connect($dbhost, $dbuser, $dbpassword, $dbname);

if(!$connect){
    echo "Error: " . mysqli_connect_error();
    exit();
}

echo "Connection Success!<br><br>";

$Latitude = $_GET["Lat"];
$Longitude = $_GET["Lngd"];
$Altitude = $_GET["Alt"];
$Satellite = $_GET["Sat"];


$query = "INSERT INTO GPS_coordinates (Lat,Lngd,Alt,Sat) VALUES ('$Latitude','$Longitude','$Altitude','$Satellite')";
$result = mysqli_query($connect,$query);

if($result){
    echo "success";
}else {echo "Insertion Failed!<br>"; echo mysqli_error($connect);}

?>