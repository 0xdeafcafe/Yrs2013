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
				<a id="logo" href="/#Features">
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
						<a id="nav-create" href="cities.php">Cities</a>
						<div class="identicationTag"></div>
					</li>
					<li>
						<a id="nav-about" href="about.php" class="selected">About</a>
						<div class="identicationTag"></div>
					</li>
				</ul>
			</div>

			<div id="app-info">
				<div>Xenon: YRS 2013 Project</div>
			</div>
		</div>
		<div id="scrollableContentContainer" class="layoutFluidRightSection">
			<div class="mainPageContent" ng-view>	
				<h1>Overview</h1>
				<p>For YRS 2013 we decided to make a hack that involved a development Xbox 360, Kinect, location data, and a copy of Halo 4.</p>
				<p></p>
				<p>We take a base set of city data, and in real-time via the Twitter API, we modify gameplay values of Halo 4. These values are values such as character speed and power. We use Twitter to pull tweets and perform semantic analysis to determine whether a tweet is positive or negative about the city of London or Birmingham, and add gameplay values to the teams of these cities.</p>
				<p></p>
				<h1>Technical</h1>
				<p></p>
				<p>For the 360 side we used C++ and C# along with an edited Kinect SDK sample to launch into Halo 4, and for the web frontend we used HTML, CSS and PHP that communicates with a MySQL database that will pull data the same data that the game reads. This data is then parsed into JSON which the database will then read and post to the Cities page.</p>
				<p></p>
				<h1>Who built this?</h1>
				<ul>
					<li><a href="http://twitter.com/alexerax">Alex</a></li>
					<li><a href="http://twitter.com/RichardR">Richard</a></li>
					<li><a href="http://twitter.com/Matchi_chan">Lucy</a></li>
					<li><a href="http://twitter.com/SimionPutina">Simion</a></li>
					<li><a href="http://twitter.com/Jewllama">Gideon</a></li>
					<li><a href="http://twitter.com/eve3208">Eve</a></li>
				</ul>
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
