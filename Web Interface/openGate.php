<?php

$fp = fopen ("COM3:", "r+");
if (!$fp) {
   echo "Radio busy, refresh page in a few seconds.";
   return;
}

$motorSteps = $_GET['steps'];

fwrite($fp, "@open ".$motorSteps."@");

fclose($fp);

#Sleep based on motorSteps, at least 500microseconds*motorSteps/1000000
$sleepTime = 500.0*((float)$motorSteps);
$sleepTime = $sleepTime/1000000.0;
$sleepTime = $sleepTime + 1.0;

echo '<div class="row">Waited '.$sleepTime.' seconds for door.</div>';

sleep($sleepTime);

echo '<script>document.location.replace("/");</script>';


?>