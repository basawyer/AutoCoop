function read(a)
{
    $("#qr-value").text(a);
    
    var theUrl = "http://zamba.cs.vt.edu/DriveTech/checkUsername.php?uid="+a;
    var xmlHttp = null;

    xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", theUrl, false );
    xmlHttp.send( null );
    var response = xmlHttp.responseText;
    
    if(response == "1"){
    
    	self.location="http://zamba.cs.vt.edu/DriveTech/completeStation1.php?uid="+a;
    }
}
    
qrcode.callback = read;