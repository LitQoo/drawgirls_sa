<?php
include "header.php";

if(!$_GET['where'])$_GET['where']="";
if(!$_GET['limit'])$_GET['limit']=100;
if(!$_GET['sort'])$_GET['sort']="no desc";

?>
<center>
<form action=<?=$_SERVER["PHP_SELF"]?>>
where : <input name='where' size="50" value='<?=$_GET['where']?>'><br>
sort : <input name='sort' size="50" value='<?=$_GET['sort']?>'><br>
limit : <input name='limit' size="50" value='<?=$_GET['limit']?>'><br>
 <input type=submit value="확인"><br>
 
 현재시각 : <?php 
echo TimeManager::get()->getCurrentDateString();
 ?>
</form>
</center>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getST("message")?>" dbSort="<?=$_GET["sort"]?>" dbLimit='<?=$_GET["limit"]?>' dbWhere='<?=$_GET["where"]?>' border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
         <th field="nick" viewer="text">nick</th>
			<th field="memberID" viewer="text">memberID</th>
         <th field="friendNick" viewer="text">friendNick</th>
         <th field="friendID" viewer="text">friendID</th>
			<th field="content" viewer="text" viewerOption='{"cut":100}' editor="textarea">content</th>
			<th field="type" viewer="text">type</th>
         <th field="isSendMsg" viewer="text">isSendMsg</th>
			<th field="regDate" viewer="text">regDate</th>
         <th field="regTime" viewer="text">regTime</th>
         <th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>