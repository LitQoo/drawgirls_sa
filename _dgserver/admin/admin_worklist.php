<?php
include "header.php";

if(!$_GET['where'])$_GET['where']="";
if(!$_GET['limit'])$_GET['limit']=1000;
if(!$_GET['sort'])$_GET['sort']="state asc, worker asc,`order` desc, no desc";


?>
<script>

var stateBg = function(value,option){
	if(value=="대기"){
		return "<b><font color=blue>대기</font></b>";
	}else if(value=="ing"){
		return "<b><font color=red>ING</font></b>";

	}else if(value=="완료"){
		return "<b><font color=green>완료</font></b>";
	}else if(value=="alert"){
		return "<b><font color=pink>알림</font></b>";

	}else{
		return "<b><font color=orange>취소</font></b>";

	}
}

</script>



<center>
<form action=<?=$_SERVER["PHP_SELF"]?>>


<table border=1>
	<tr>
		<td bgcolor=cccccc><b>담당자</b></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>전체</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker%3D""&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>미정</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker+like+"%25현수%25"&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>현수</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker+like+"%25경수%25"&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>경수</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker+like+"%25영호%25"&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>영호</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker+like+"%25정순%25"&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>정순</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker+like+"%25남희%25"&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>남희</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker+like+"%25승용%25"&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>승용</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?where=worker+like+"%25민수%25"&sort=<?=$_GET['sort']?>&limit=<?=$_GET['limit']?>'>민수</a></td>
	</tr>
</table>

<table border=1>
	<tr>
		<td bgcolor=cccccc><b>타입</b></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?sort=<?=$_GET['sort']?>&where=&limit=<?=$_GET['limit']?>'>전체</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?sort=<?=$_GET['sort']?>&where=type%3D"수정"&limit=<?=$_GET['limit']?>'>수정</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?sort=<?=$_GET['sort']?>&where=type%3D"버그"&limit=<?=$_GET['limit']?>'>버그</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?sort=<?=$_GET['sort']?>&where=type%3D"추가"&limit=<?=$_GET['limit']?>'>추가</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?sort=<?=$_GET['sort']?>&where=type%3D"디자인"&limit=<?=$_GET['limit']?>'>디자인</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?sort=<?=$_GET['sort']?>&where=type%3D"회의"&limit=<?=$_GET['limit']?>'>회의</a></td>
		<td><a href='<?=$_SERVER["PHP_SELF"]?>?sort=<?=$_GET['sort']?>&where=type%3D"기타"&limit=<?=$_GET['limit']?>'>기타</a></td>
	</tr>
</table>


<br>
where : <input name='where' size="50" value='<?=$_GET['where']?>'><br>
sort : <input name='sort' size="50" value='<?=$_GET['sort']?>'><br>
limit : <input name='limit' size="50" value='<?=$_GET['limit']?>'><br>
 <input type=submit value="확인"><br>



<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_WORKLIST?>" dbSort="<?=$_GET["sort"]?>" dbLimit='<?=$_GET["limit"]?>' dbWhere='<?=$_GET["where"]?>' border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="type" viewer="text" editor="select" editorOption='["수정","버그","추가","디자인","회의","기타"]'>타입(type)</th>
			<th field="order" viewer="text" editor="text" editorOption='{"datatype":"int"}'>우선순위(order)</th>
			<th field="writer" viewer="text" editor="text">제안자(writer)</th>
			<th field="content" viewer="text" editor="textarea">내용(content)</th>
			<th field="worker" viewer="text" editor="text">담당자(worker)</th>
			<th field="result" viewer="text" editor="textarea">결과(result)</th>
			<th field="state" viewer="custom" viewerOption="stateBg" editor="select" editorOption='["대기","ing","완료","취소","alert"]'>상태(state)</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>