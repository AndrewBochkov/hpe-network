<html>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<body>

<?php
require ("library.php.inc"); //.. Library

$choices=$_POST['sz'];
$dates = date("d.m.Y H:i:s");
$files=	$_SERVER["HTTP_REFERER"];
$ip = $_SERVER["REMOTE_ADDR"];
$referer=$_SERVER["HTTP_REFERER"];

list ( $Description, $Products ) = getSession( );
if ( $Products[0] == 0 ) {
	print "Need to return to previusly page<br>"; 
	return;
} 

//.. TEST::print "before searchFor()<br>\n"; print_r ( $choices ); print "before2 searchFor()<br>\n";
$mSearch = searchFor( $choices, $Products ); //... Result or 0
//.. TEST:: print "after searchFor()<br>\n"; print_r ( $mSearch);
if ( $mSearch[0] == 0 ) {
	print "No records find<br>"; 
} else searchOut( $Description, $mSearch, $choices ); //... Print array with result

?>


</BODY>
</HTML>
