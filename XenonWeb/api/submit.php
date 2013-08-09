<?php
	// grabs post from url and decodes json into an array
	$programIn = file_get_contents('php://input');
	$incoming_data = json_decode($programIn, TRUE);
	
	// string data to go into database
	$string_data = json_encode($programIn['Data Points']);
	
	// database stuff goes here
	$con = mysqli_connect('localhost', 'arborvi1_gideon', 'harris','arborvi1_cw');
	$query = "INSERT INTO xenon ('city_a', 'city_b', 'victor', 'raw') VALUES ('$string_data')";
	$result = mysqli_query($con, $query);
?>