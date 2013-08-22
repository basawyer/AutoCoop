<?php

$timeout = 3;
$response="";
$old = ini_set('default_socket_timeout', $timeout);
$fp = fopen ("COM3:", "r+") or die('"Radio busy, refresh page in a few seconds.');
ini_set('default_socket_timeout', $old);
if (!$fp) {
   echo "Radio busy, refresh page in a few seconds.";
   return;
}
stream_set_timeout($fp, $timeout);
stream_set_blocking($fp, 0);

?>

<html>
<head>
	<meta charset="utf-8"/>
	<title>AutoCoop Interface</title>
	<link href="css/bootstrap.min.css" rel="stylesheet" media="screen">
	<link rel="stylesheet" href="css/iThing.css"> 
	
	<script src="js/jquery-1.7.1.min.js"></script>
  <script src="js/jquery-ui-1.8.16.custom.min.js"></script>
  
  <meta http-equiv="Refresh" content="59">
  
</head>
<body>
	<div class="container span8"><h3 style="text-align: center">Connected to AutoCoop!</h3></div>
	<div class="hero-unit">
	
<?php

fwrite($fp, "@status@");
sleep(1);


while(!feof($fp)){
    $response.=stream_get_contents($fp, 2);
}

fclose($fp);

if(strlen($response) > 0 && $response[0] == '?' && $response[strlen($response)-1] == '?'){
	
	$split = explode("-",substr($response, 1, -1));
	$battery = $split[0];
	$open = $split[1];
	
	if($open == "0"){
	
		echo '<div class="row"><h3 class="offset2" style="color: red">DOOR CLOSED</h3></div>';
	
	}
	else{
	
		echo '<div class="row"><h3 class="offset2" style="color: green">DOOR OPENED</h3></div>';
	}
	
	
	echo '<div class="row"><div class="span2 offset2"> <h4>Battery: '.$battery.'%</h4> </div>';

	if($open == "0"){
?>
		<div class="span2"><form name="openGate" method="post" action="openGate.php?steps=8750">
			<button type="submit" class="btn btn-primary">Open Gate</button>
		</form></div>
		</div>
	
		<div class="row"><img src="img/doorClosed.jpg"></div>

<?php

	}
	else{
		
?>
		<div class="span2"><form name="closeGate" method="post" action="closeGate.php?steps=8750">
			<button type="submit" class="btn btn-primary">Close Gate</button>
		</form></div>
		</div>
			
		<div class="row"><img src="img/doorOpen.jpg"></div>
	
<?php

	}

}
else{

	echo "Got bad reponse from AutoCoop.  Please refresh in a few seconds.";
}

?>

	</div>
</body>
</html>