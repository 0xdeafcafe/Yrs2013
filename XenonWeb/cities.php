<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8" />
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Xenon</title>
	<link rel="stylesheet" type="text/css" href="styles/reset.css">
	<link rel="stylesheet" type="text/css" href="styles/application.css">
</head>

<body>
	<div id="wrapper" class="layoutWrapper">
		<div id="leftColumn" class="layoutFixLeftSection">
			<div class="layoutFixTopLeftSection">
				<a id="logo" href="index.php">
					<div id="logo-img">Logo goes here</div>
				</a>
			</div>

			<hr class="position-relative" />			

			<div id="navigation">
				<ul class="navigationList navigationMenu">
					<li>
						<a id="nav-feat" href="index.php">Home</a>
						<div class="identicationTag"></div>
					</li>
					<li>
						<a id="nav-create" href="cities.php" class="selected">Cities</a>
						<div class="identicationTag"></div>
					</li>
					<li>
						<a id="nav-about" href="about.php">About</a>
						<div class="identicationTag"></div>
					</li>
				</ul>
			</div>

			<div id="app-info">
				<div>Xenon: YRS 2013 Project</div>
			</div>
		</div>
		<div id="scrollableContentContainer" class="layoutFluidRightSection">
			<div class="mainPageContent">	
				<table class="table">
					<thead>
						<tr>
							<th>City A </th>
							<th>City B </th>
							<th>Victor </th>
							<th>Raw </th>
						</tr>
					</thead>
					<tbody>
						<?php
							$con = mysqli_connect("localhost", "arborvi1_gideon", "harris", "arborvi1_cw"); 
							$query = "SELECT * FROM xenon ORDER BY citiesid DESC LIMIT 30";
							$result = mysqli_query($con, $query);
							while ($row = mysqli_fetch_array($result, MYSQLI_NUM)) {
								echo "<tr>";
								echo "<td>" . $row[1] . "</td>";
								echo "<td>" . $row[2] . "</td>";
								echo "<td>" . $row[3] . "</td>";
								echo "<td>" . $row[4] . "</td>";
								echo "</tr>";
							}
						?>
					</tbody>
				</table>
			</div>
		</div>

		<div id="modalMask"></div>
		<div id="pendingMask"></div>
		<div id="modal" class="error">
			<h1>Modal Example</h1>	
			<p>
				Lorem ipsum dolor sit amet, consectetur adipiscing elit fusce vel sapien elit in malesuada semper mi, id sollicitudin urna fermentum ut fusce varius nisl ac ipsum gravida vel pretium tellus tincidunt integer eu augue augue nunc elit dolor, luctus placerat scelerisque euismod, iaculis eu lacus nunc mi elit, vehicula ut laoreet ac, aliquam sit amet justo nunc tempor, metus vel placerat suscipit, orci nisl iaculis eros, a tincidunt nisi odio eget lorem nulla condimentum tempor mattis ut vitae feugiat augue cras ut metus a risus iaculis scelerisque eu ac ante fusce non varius purus aenean nec magna felis fusce vestibulum.
			</p>
			<div class="actions">
				<input type="button" value="Close" />
			</div>
		</div>
	</div>
</body>
</html>
