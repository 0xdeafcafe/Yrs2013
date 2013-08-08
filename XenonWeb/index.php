<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8" />
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Xenon</title>
	<link rel="stylesheet" type="text/css" href="styles/reset.css">
	<link rel="stylesheet" type="text/css" href="styles/application.css">
	<script type="text/javascript" src="jquery-1.10.2.min.js"></script>
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
						<a id="nav-feat" href="" class="selected">Home</a>
						<div class="identicationTag"></div>
					</li>
					<li>
						<a id="nav-create" href="cities.php">Cities</a>
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
			<div class="mainPageContent" ng-view>	
				<h1>Welcome to Xenon</h1>
				<p>Cities contains the logs from the program and About explains what the program is.</p>
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
