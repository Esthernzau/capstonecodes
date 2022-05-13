<?php
	/* Database connection settings */
	$host = 'localhost';
	$user = 'root';
	$pass = '';
	$db = 'capstone';
	$mysqli = new mysqli($host,$user,$pass,$db) or die($mysqli->error);

 	$coordinates = array();
 	$latitudes = array();
 	$longitudes = array();

	// Select all the rows in the markers table
	$query = "SELECT  `Lat`, `Lngd` FROM `gps_coordinates` ";
	$result = $mysqli->query($query) or die('data selection for google map failed: ' . $mysqli->error);

 	while ($row = mysqli_fetch_array($result)) {

		$latitudes[] = $row['Lat'];
		$longitudes[] = $row['Lngd'];
		$coordinates[] = 'new google.maps.LatLng(' . $row['Lat'] .','. $row['Lngd'] .'),';
		echo $coordinates[0];
	}

	//remove the comaa ',' from last coordinate
	$lastcount = count($coordinates)-1;
	$coordinates[$lastcount] = trim($coordinates[$lastcount], ",");	
?>

<!DOCTYPE html>
<html>
	<head>
    	<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<link rel="stylesheet" type="text/css" href="style.css">
		<title>Map | View</title>
	</head>

	<body>
	    <nav>  
			<ul> 
				<li class="active"><a href="#"><img src="img/map.png"></a></li>
				<li><a href="#"><img src="img/logout.png"></a></li>
			</ul> 
		</nav>

		 <div class="outer-scontainer">
	        <div class="row">
	            <form class="form-horizontal" action="" method="post" name="frmCSVImport" id="frmCSVImport" enctype="multipart/form-data">
	            	<div class="form-area">	      
    					<button type="submit" id="submit" name="import" class="btn-submit">RELOAD DATA</button><br />
					</div>
	            </form>
	        </div>

		<div id="map" style="width: 100%; height: 80vh;"></div>

		<script>
			function initMap() {
			  var mapOptions = {
			    zoom: 18,
			    center: {<?php echo'lat:'. $latitudes[5.7621998] .', lng:'. $longitudes[-0.2239771] ;?>}, //{lat: --- , lng: ....}
			    mapTypeId: google.maps.MapTypeId.SATELITE
			  };

			  var map = new google.maps.Map(document.getElementById('map'),mapOptions);

			  var RouteCoordinates = [
			  	<?php
			  		$i = 0;
					while ($i < count($coordinates)) {
						echo $coordinates[$i];
						$i++;
					}
			  	?>
			  ];

			 //  var RoutePath = new google.maps.Polyline({
			  //   path: RouteCoordinates,
			  //   geodesic: true,
			  //   strokeColor: '#1100FF',
			  //   strokeOpacity: 1.0,
			  //   strokeWeight: 10
			  // });

			  mark = 'img/mark.png';
			  flag = 'img/flag.png';

			  startPoint = {<?php echo'lat:'. $latitudes[5.7621998] .', lng:'. $longitudes[-0.2239771] ;?>};
			  endPoint = {<?php echo'lat:'.$latitudes[$lastcount] .', lng:'. $longitudes[$lastcount] ;?>};
			
			  var marker = new google.maps.Marker({
			  	position: startPoint,
			  	map: map,
			  	icon: mark,
			  	title:"Start point!",
			  	animation: google.maps.Animation.DROP
			  });

			  var marker = new google.maps.Marker({
			  position: endPoint,
			   map: map,
			   icon: flag,
			   title:"End point!",
			   animation: google.maps.Animation.BOUNCE
			});

			  RoutePath.setMap(map);
			}

			google.maps.event.addDomListener(window, 'load', initialize);
    	</script>

    	<!--remenber to put your google map key-->
	    <script async defer src="https://maps.googleapis.com/maps/api/js?key=AIzaSyCX0KsCRQHLZsSb6ZfdnaPb_ad1GHd5J3s&callback=initMap"></script>

	</body>
</html>