<?php
include "../lib.php";


?>

<html>
<head>
<title>
<?php
if($title)echo $title;
else {
$php_self = explode("/", $_SERVER[PHP_SELF]);
$url_cnt = count($php_self) - 1;
$this_page = $php_self[$url_cnt];
$this_page_name = explode(".",$this_page);
$this_page_name = $this_page_name[count($this_page_name)-2];
$lname = explode("_",$this_page_name);
echo $lname[1];
}
?>

</title>
		<script src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
		<script src="lib.js"></script>
		<script src="LQDataTable.js"></script>
		<!-- Latest compiled and minified CSS -->

<!-- Bootstrap styles -->
<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">
<!-- Generic page styles -->
<link rel="stylesheet" href="jqueryupload/css/style.css">
<!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
<link rel="stylesheet" href="jqueryupload/css/jquery.fileupload.css">

<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
</head>
<body>






<?php if(!$menuoff){ ?>
<table border=1 align=center>
<tr>
<td>

<table border=1>
<tr>
<td bgcolor=cccccc><b>System</b></td>
<td><a href=admin_help.php>api<br><font color=red>에이피아이</font></a></td>
<td><a href=admin_util.php>util<br><font color=red>유틸</font></a></td>
<td><a href=admin_statsClear.php>stats-clear<br><font color=red>통계</font></a></td>
<td><a href=admin_worklist.php>worklist<br><font color=red>작업목록</font></a></td>
</tr>
</table>

<table border=1>
<tr>
<td bgcolor=cccccc><b>MainServer</b></td>
<td><a href=admin_commonSetting.php>commonSetting<br><font color=red>커먼세팅</font></a></td>
<td><a href=admin_keyintvalue.php>key-intvalue<br><font color=red>키인트벨류</font></a></td>
<td><a href=admin_notice.php>notice<br><font color=red>공지</font></a></td>
<td><a href=admin_puzzle.php>puzzle<br><font color=red>퍼즐</font></a></td>
<td><a href=admin_stage.php>stage<br><font color=red>스테이지</font></a></td>
<td><a href=admin_card.php>card<br><font color=red>카드</font></a></td>
<td><a href=admin_character.php>character<br><font color=red>캐릭터</font></a></td>
<td><a href=admin_item.php>item<br><font color=red>아이템</font></a></td>
<td><a href=admin_shop.php>shop<br><font color=red>상점</font></a></td>
<td><a href=admin_language.php>language<br><font color=red>언어</font></a></td>
<td><a href=admin_mission.php>mission<br><font color=red>미션</font></a></td>
<td><a href=admin_monster.php>monster<br><font color=red>몬스터</font></a></td>
<td><a href=admin_movement.php>movement<br><font color=red>움직임</font></a></td>
<td><a href=admin_pattern.php>pattern<br><font color=red>패턴</font></a></td>
<td><a href=admin_images.php>images<br><font color=red>이미지</font></a></td>
<td><a href=admin_resources.php>resources<br><font color=red>리소스</font></a></td>
</tr>
</table>

<table border=1>
<tr>
<td bgcolor=cccccc><b>ShardServer</b></td>
<td><a href=admin_user.php>user<br><font color=red>유저</font></a></td>
<td><a href=admin_log.php>log<br><font color=red>로그</font></a></td>
<td><a href=admin_message.php>message<br><font color=red>메세지</font></a></td>
<td><a href=admin_weeklyScore.php>weeklyscore<br><font color=red>주간점수</font></a></td>
<td><a href=admin_stageScore.php>stagescore<br><font color=red>스테이지점수</font></a></td>
</tr>
</table>

</td></tr></table>
<br>
<?php } ?>