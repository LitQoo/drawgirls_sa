<?php
include "header.php";
?>

<script>
	function filesize (url) {
    var req = window.ActiveXObject ? new ActiveXObject("Microsoft.XMLHTTP") : new XMLHttpRequest();
    if (!req) throw new Error('XMLHttpRequest not supported');
    
    req.open ('HEAD', url, false);
    req.send (null);
    
    if (!req.getResponseHeader) {
        try {
            throw new Error('No getResponseHeader!');
        } catch(e){
            return false;
        }
    } else if (!req.getResponseHeader('Content-Length')) {
        try {
            throw new Error('No Content-Length!');
        } catch(e){
            return false;
        }
    } else {
        return req.getResponseHeader('Content-Length'); 
    }
    }
    
		$(document).ready(function() {
			$("body").on("click",".selectImage", function(e){ //user click on remove text
				imgurl = $(this).attr('imgname')
				fsize = filesize(imgurl)/1024;
				
				imgurl = imgurl.replace("../images/", "");
				
				log("select"+imgurl+" size:"+fsize);
				
				opener.changeJsonFormInput(<?php echo $_GET[delkey]; ?>,imgurl,fsize);

		        //$(opener.document).find("#{{ input }}").val($(this).prop('src')).change();
		        window.close();
				return false;
			}) 

			$('#imageupload').on("change", function(){ 
				var file = $(this)[0].files[0];
				$("#imagename").val(file.name);

			 });
			

		});


	var imageViewFunc = function(value,option){
		return "<img src='../images/"+value+"' class='selectImage' imgname='"+value+"' width=100>";
	}

</script>





<table border="1" align="center" width=800>
	<tr><td>

<?php
$categorys = array("card","monster","sil","item","mission","pattern","theme","thumnail","etc");


$uploaderForm = '<select name="category" id="imageCategory">';

for($i=0;$i<count($categorys);$i++){
	$uploaderForm.="<option value=".$categorys[$i].">".$categorys[$i]."</option>";
}

$uploaderForm.="</select>";


?>


<?php 

include "jqueryupload/uploader.html";
 
?>

	</td></tr>
</table>
<br>

<table border=1 align=center>
<tr>
<?php

echo"<td><a href=".$_SERVER["PHP_SELF"].">all</a></td>";
	
for($i=0;$i<count($categorys);$i++){
	echo"<td><a href=".$_SERVER["PHP_SELF"]."?category=".$categorys[$i].">".$categorys[$i]."</a></td>";
}

?>

<td>
</table>
<table align=center border=1><tr><td>
<?php
$realDir="../images";
if($_GET["category"])$realDir.="/".$_GET["category"];
$i=0;
foreach (filesInDir ($realDir) as $key => $a_file){ 
	$i++;
	echo "<img src='".$a_file ."' width='200' class='selectImage' imgname='".$a_file."'>" ; 
	
	if($i%5==0)echo"<br>";
} 
?>
</td></tr></table>



<?php
include "footer.php";
?>


