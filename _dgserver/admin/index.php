<?php

include "../lib.php";





$n = strtotime("next Sunday");
$c = time();
$m = $n-$c;
$m2 = $m/(60*60); //몇시간남음
$m3 = $m2/24; //몇일남음
echo "다음주 일요일까지 남은시간".$n."<br>";
echo "<br>";
echo "현재시간 ".$c;
echo "<br>";
echo ($m2)."시간 남았습니다.";
echo "<br>";
echo ($m3)."시간 남았습니다.";
echo "<br>";
?>




<?php
// reloadPuzzleInfo();
// exit;
?>

<?php
// $boss[hp] = 10;
// $boss[agi] = 10;


// 레벨설정하기
// $levels = array(0,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8);

// $result = mysql_query("select * from aStageTable where no<1000 order by no asc",DBManager::get()->getMainConnection());

// $level=1;
// while($data = mysql_fetch_array($result)){


// //	echo "update aStageTable set level=$level where no=".$data[no]."<br>";
// 	mysql_query("update aStageTable set level=$level where no=".$data[no],DBManager::get()->getMainConnection());
// 	$levels[$level]-=1;
// 	if($levels[$level]<=0){
// 		$level++;
// 	}


// }

// exit;
?>

<?php
/*
//미션세팅~!
$missionList[1]=json_decode('{"type":1,"option":{}}',true); // 용감한기사
$missionList[2]=json_decode('{"type":2,"option":{"count":3}}',true); // 사냥꾼
$missionList[3]=json_decode('{"type":3,"option":{"percent":20,"count":2}}',true); // 욕심쟁이
$missionList[4]=json_decode('{"type":4,"option":{"count":20}}',true); // 수집가
$missionList[5]=json_decode('{"type":5,"option":{"percent":90}}',true); // 완벽주의자
$missionList[6]=json_decode('{"type":6,"option":{}}',true); // 결벽주의자
$missionList[7]=json_decode('{"type":7,"option":{"sec":70}}',true); // 비지니스맨


$usingMission=array(1,2,3,4,7);

//아이템 세팅하기
$itemList = array();

{//대쉬
	$item["data"] = '{"type":4,"currency":"gold","price":3000,"option":{"sec":3}}';
	$item["type"] = "normal";
	$itemList[] = $item;
}
{//필살기
	$item["data"] = '{"type":5,"currency":"gold","price":1000,"option":{}}';
	$item["type"] = "normal";
	$itemList[] =$item;
}

{//부하몹원킬
	$item["data"] = '{"type":6,"currency":"gold","price":2000,"option":{}}';
	$item["type"] = "normal";
	$itemList[] =$item;
}

{//아이템두배
	$item["data"] = '{"type":7,"currency":"gold","price":2000,"option":{"percent":1}}';
	$item["type"] = "normal";
	$itemList[] =$item;
}

{//침묵
	$item["data"] = '{"type":8,"currency":"gold","price":1000,"option":{"sec":10}}';
	$item["type"] = "normal";
	$itemList[] =$item;
}

{//부하몹없이시작
	$item["data"] = '{"type":9,"currency":"gold","price":1000,"option":{}}';
	$item["type"] = "normal";
	//$itemList[] =$item;
}

{//전체시간추가
	$item["data"] = '{"type":10,"currency":"gold","price":1000,"option":{"sec":30}}';
	$item["type"] = "mission";
	$itemList[] = $item;
}


{ //보스체력감소
	$item["data"] = '{"type":11,"currency":"gold","price":2000,"option":{"percent":30}}';
	$item["type"] = "normal";
	$itemList[] =$item;
}

{//부하몹작게
	$item["data"] = '{"type":12,"currency":"ruby","price":10,"option":{"percent":20}}';
	$item["type"] = "mission";
	$itemList[] =$item;
}

{//욕심쟁이치료
	$item["data"] = '{"type":13,"currency":"ruby","price":10,"option":{"percent":5}}';
	$item["type"] = "mission";
	$itemList[] =$item;
}

{//완벽주의치료
	$item["data"] = '{"type":14,"currency":"ruby","price":10,"option":{"percent":2}}';
	$item["type"] = "mission";
	//$itemList[] =$item;
}

{//결벽증치료
	$item["data"] = '{"type":15,"currency":"ruby","price":10,"option":{}}';
	$item["type"] = "mission";
	//$itemList[] =$item;
}

{//친구카드빌리기
	$item["data"] = '{"type":16,"currency":"social","price":10,"option":{}}';
	$item["type"] = "special";
	$itemList[] =$item;
}

$defaultItemStr = '[{"type":1,"option":{"power":10}},{"type":2,"option":[]},{"type":3,"option":{"sec":10}}]';



$nitemList = array();
$gitemList = array();

for($i=0;$i<count($itemList);$i++){
	if($itemList[$i]["type"]=="normal"){
		$nitemList[]=json_decode($itemList[$i]["data"],true);
	}else if($itemList[$i]["type"]=="mission"){
		$gitemList[]=json_decode($itemList[$i]["data"],true);
	}
}

echo "<pre>";
var_dump($nitemList);
var_dump($gitemList);

$sitem = json_decode('{"type":16,"currency":"social","price":10,"option":{}}',true);


$result = mysql_query("select * from $TABLE_STAGE where puzzle>=2 and no < 10000 order by no asc",DBManager::get()->getMainConnection());
srand(1);
$missionCntCheck = array(0,0,4,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
$missionCnt=0;
$oldPuzzle=0;
while($sData = mysql_fetch_assoc($result)){
	
	$puzzleNo = $sData["puzzle"];
	$missionCnt++;

	shuffle($nitemList);
	shuffle($gitemList);


	$newitem = array_slice($nitemList, 0,6);x

	$mission="0";
	
	if($oldPuzzle!=$puzzleNo){
		$missionCnt=0;
	}

	if($missionCnt==$missionCntCheck[$puzzleNo] && $missionCntCheck[$puzzleNo]!=0){
		$missionCnt=0;
		//미션배정하기
		$gi = $gitemList[0];
		$newitem[]=$gi;
		if($gi["type"]==10)$mission=7;
		//if($gi["type"]==11)$mission=1;
		if($gi["type"]==12)$mission=2;
		if($gi["type"]==13)$mission=3;
		if($gi["type"]==14)$mission=5;
		if($gi["type"]==15)$mission=6;

		echo "<font color=red>mission</font>";

	}else{
		//$newitem[] = $nitemList[6];
	}	

	$newitem[]=$sitem;

	$shopItemStr = json_encode($newitem,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);


	$missionStr = json_encode($missionList[$mission],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	if($mission==0)$missionStr = '{"type":0}';
	echo ("update $TABLE_STAGE set shopItems='".$shopItemStr."',defItems='".$defaultItemStr."',mission='".$missionStr."',playtime=100 where no = ".$sData[no]); //,DBManager::get()->getMainConnection()
	mysql_query("update $TABLE_STAGE set shopItems='".$shopItemStr."',defItems='".$defaultItemStr."',mission='".$missionStr."',playtime=100 where no = ".$sData[no],DBManager::get()->getMainConnection()); 
	echo mysql_error();
	echo "<br><br>";

	$oldPuzzle = $puzzleNo;

	//kvManager::increase("stageVer_".$sData[no]);
	//kvManager::increase("puzzleVer_".$sData[puzzle]);
}
exit;
*/
?>


<?php
///////////////////////// 벨런스 맞추기 ////////////////////////////////////
srand(1);
$cPattern = array();
{
$crash=array();
$crash["data"] = json_decode('{"atype":"crash","target":"yes","number":7,"area":13,"speed":50,"pattern":"112"}',true);
$crash["option"] = "number";
$crash["option_max"] = 7;
$crash["option_min"] = 1;
$crash["option_type"] = "int";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"atype":"crash","target":"yes","speed":300,"crashsize":8,"pattern":"105"}',true);
$crash["option"] = "speed";
$crash["option_max"] = 800;
$crash["option_min"] = 200;
$crash["option_type"] = "int";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"atype":"crash","speed":100,"cushioncount":3,"number":3,"area":20,"target":"no","pattern":"9"}',true);
$crash["option"] = "number";
$crash["option_max"] = 7;
$crash["option_min"] = 2;
$crash["option_type"] = "int";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"atype":"crash","target":"yes","crashframe":180,"crashsize":50,"pattern":"107"}',true);
$crash["option"] = "crashsize";
$crash["option_max"] = 50;
$crash["option_min"] = 20;
$crash["option_type"] = "int";
$crash["castframe"]="crashframe";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"atype":"crash","target":"yes","pattern":"113"}',true);
$crash["option"] = "";
$crash["option_type"] = "int";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"atype":"crash","target":"yes","number":8,"speed":200,"crashsize":10,"pattern":"106"}',true);
$crash["option"] = "number";
$crash["option_max"] = 8;
$crash["option_min"] = 2;
$crash["option_type"] = "int";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"targetingframe":180,"shootframe":300,"atype":"crash","target":"yes","castframe":0,"pattern":"1010"}',true);
$crash["option"] = "shootframe";
$crash["option_max"] = 300;
$crash["option_min"] = 60;
$crash["option_type"] = "int";
$crash["castframe"]="targetingframe";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"atype":"crash","number":5,"area":50,"target":"no","pattern":"1013"}',true);
$crash["option"] = "number";
$crash["option_max"] = 5;
$crash["option_min"] = 1;
$crash["option_type"] = "int";
$cPattern[]=$crash;
}

{
$crash=array();
$crash["data"] = json_decode('{"totalframe":300,"atype":"crash","target":"no","pattern":"109"}',true);
$crash["option"] = "totalframe";
$crash["option_max"] = 600;
$crash["option_min"] = 300;
$crash["option_type"] = "int";
$cPattern[]=$crash;
}


$sPattern=array();

{
	$pattern=array();
	$pattern["data"] = json_decode('{"atype":"special","target":"yes","totalframe":400,"pattern":"1001"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 6000;
	$pattern["option_min"] = 600;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}

{
	$pattern=array();
	$pattern["data"] = json_decode('{"atype":"special","target":"no","totalframe":300,"pattern":"1002"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 600;
	$pattern["option_min"] = 300;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}

{
	$pattern=array();
	$pattern["data"] = json_decode('{"atype":"special","target":"no","totalframe":240,"pattern":"1003"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 1200;
	$pattern["option_min"] = 300;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}

{
	$pattern=array();
	$pattern["data"] = json_decode('{"target":"yes","atype":"special","pattern":"1009"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 300;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"atype":"special","target":"yes","totalframe":180,"pattern":"1004"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 300;
	$pattern["option_min"] = 120;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"atype":"special","target":"yes","pattern":"1005"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 240;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"atype":"special","target":"no","totalframe":300,"pattern":"1008"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 600;
	$pattern["option_min"] = 300;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"atype":"special","target":"yes","totalframe":300,"pattern":"1006"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 600;
	$pattern["option_min"] = 300;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"totalframe":800,"atype":"special","target":"no","pattern":"1015"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 1800;
	$pattern["option_min"] = 300;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"totalframe":300,"area":5,"target":"yes","atype":"special","movingframe":120,"pattern":"1012"}',true);
	$pattern["option"] = "area";
	$pattern["option_max"] = 30;
	$pattern["option_min"] = 5;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"totalframe":300,"target":"yes","atype":"special","castframe":60,"pattern":"1011"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 600;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"remainsecond":9,"area":50,"target":"no","atype":"normal","pattern":"1016"}',true);
	$pattern["option"] = "area";
	$pattern["option_max"] = 70;
	$pattern["option_min"] = 20;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}


{
	$pattern=array();
	$pattern["data"] = json_decode('{"totalframe":300,"shootframe":30,"speed":250,"atype":"special","target":"no","pattern":"1014"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 600;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}

{
	$pattern=array();
	$pattern["data"] = json_decode('{"remainsecond":9,"area":50,"target":"no","atype":"normal","pattern":"1016"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 600;
	$pattern["option_min"] = 180;
	$pattern["option_type"] = "int";
	$sPattern[]=$pattern;
}

$mPattern=array();

{
	$pattern=array();
	$pattern["data"] = json_decode('{"oneshot":1,"oneshotterm":0,"gunnumber":4,"targettype":2,"degreev":0,"color":1,"totalframe":180,"castframe":90,"speed":180,"fireterm":4,"totaldegree":360,"percent":1,"target":"no","atype":"normal","pattern":"108"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 240;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$mPattern[]=$pattern;
}
{
	$pattern=array();
	$pattern["data"] = json_decode('{"oneshot":1,"oneshotterm":0,"gunnumber":2,"targettype":2,"degreev":1,"color":2,"totalframe":180,"castframe":90,"speed":180,"fireterm":4,"totaldegree":360,"percent":1,"target":"no","atype":"normal","pattern":"108"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 240;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$mPattern[]=$pattern;
}
{
	$pattern=array();
	$pattern["data"] = json_decode('{"oneshot":5,"oneshotterm":5,"gunnumber":4,"targettype":2,"degreev":2,"color":3,"totalframe":180,"castframe":90,"speed":180,"fireterm":4,"totaldegree":360,"percent":1,"target":"no","atype":"normal","pattern":"108"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 240;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$mPattern[]=$pattern;
}
{
	$pattern=array();
	$pattern["data"] = json_decode('{"oneshot":1,"oneshotterm":0,"gunnumber":1,"targettype":1,"degreev":10,"color":4,"totalframe":180,"castframe":90,"speed":300,"fireterm":4,"totaldegree":0,"percent":1,"target":"yes","atype":"normal","pattern":"108"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 240;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$mPattern[]=$pattern;
}
{
	$pattern=array();
	$pattern["data"] = json_decode('{"oneshot":1,"oneshotterm":0,"gunnumber":4,"targettype":1,"degreev":0,"color":5,"totalframe":180,"castframe":90,"speed":180,"fireterm":4,"totaldegree":90,"percent":1,"target":"yes","atype":"normal","pattern":"108"}',true);
	$pattern["option"] = "totalframe";
	$pattern["option_max"] = 240;
	$pattern["option_min"] = 60;
	$pattern["option_type"] = "int";
	$mPattern[]=$pattern;
}

function getCardMissile($boss,$stageLevel){
	$alpabat = array("A","B","C","D","E","F","G","H","I","J","K","L","m","n","o");
	//뒷자리가 EFG면 레이저
	$arand = $alpabat[(int)rand(0,8)].$alpabat[(int)rand(0,3)];
	$prand = $boss[hp]/(float)20;
	$drand = $boss[agi];

	$speedmax = $stageLevel*3;
	if($speedmax>70)$speedmax=70;
	$srand=rand(20,$speedmax+20)/(float)10.0;

	$cardStat=array();
	for($grade=1;$grade<=3;$grade++){
				//카드 단계별 속성능력 up
		$prg=0.5;
		$drg=0.5;
		if($grade==3){
			$prg = 1.0;
			$drg = 1.0;
		}else if($grade==2){
			if(rand(0,1)){
				$prg = 0.8;
			}else{
				$drg = 0.8;
			}
		}

		
		$ms["type"]=$arand;
		$ms["power"]=(int)($prg*$prand);		
		$ms["dex"]=(int)($drg*$drand);		
		$ms["speed"]=$srand*0.5;
		
		if($ms["power"]<1)$ms["power"]=1;
		if($ms["dex"]<1)$ms["dex"]=1;
		if($ms["speed"]<0.5)$ms["speed"]=0.5;
		
		$cardStat[$grade-1]=$ms;
	}

	return $cardStat;
}


function getCrashPattern($number,$level){
	global $cPattern;
	if($level>10)$level=10;
	$pNo = $number%count($cPattern);
	$pattern = $cPattern[$pNo];
	$data = $pattern["data"];
	$frame = 150-(150-60)/10*$level;

	$optionName = $pattern["option"];
	
	if($optionName){
		$optionValue = (($pattern["option_max"]-$pattern["option_min"])/10*$level+$pattern["option_min"]);
		$data[$optionName]= (int)$optionValue;
	}

	$frameName = "castframe";
	if($pattern["castframe"]) $frameName = $pattern["castframe"];
	$data[$frameName] = $frame;


	$data["percent"]=3;

	return $data;

}

function getCrashPatternByNo($no){
	global $cPattern;
	$number = (int)($no/2);
	$level = (int)($number/count($cPattern));
	return getCrashPattern($no,$level);
}

function getSpecialPattern($number,$level){
	global $sPattern;
	if($level>10)$level=10;
	$pNo = $number%count($sPattern);
	$pattern = $sPattern[$pNo];
	$data = $pattern["data"];
	$frame = 100-(100-60)/10*$level;

	$optionName = $pattern["option"];
	
	if($optionName){
		$optionValue = ($pattern["option_max"]-$pattern["option_min"])/10*$level+$pattern["option_min"];
		$data[$optionName]= $optionValue;
	}

	$frameName = "castframe";
	if($pattern["castframe"]) $frameName = $pattern["castframe"];
	$data[$frameName] = $frame;

	$data["percent"]=3;
	return $data;

}

function getMissilePattern($number,$level){
	global $mPattern;
	if($level>10)$level=10;
	$pNo = $number%count($mPattern);
	$pattern = $mPattern[$pNo];
	$data = $pattern["data"];
	$frame = 60-(60-30)/10*$level;

	$pattern["totalframe"] = (240-60)/10*$level+60;
	$pattern["speed"]= (300-180)/10*$level+180;

	$frameName = "castframe";
	if($pattern["castframe"]) $frameName = $pattern["castframe"];
	$data[$frameName] = $frame;
	$data["color"]=rand(1,9);
	$data["percent"]=2;
	return $data;

}


function getPattern($stageNo,$stageLevel){
	//스테이지 레벨을 잘 쪼개서 값을 분배하자.
	$sType = rand(0,2);
	$high = (int)(0.5*$stageLevel);
	$low = (int)(0.25*$stageLevel);
	$cp=array();
	$sp=array();
	$mp=array();
	if($sType==0){
		$cp = getCrashPattern($stageNo,$high);
		$sp = getSpecialPattern($stageNo,$low);
		$mp = getMissilePattern($stageNo,$low,$stageLevel);
	}else if($sType==1){
		$cp = getCrashPattern($stageNo,$low);
		$sp = getSpecialPattern($stageNo,$high);
		$mp = getMissilePattern($stageNo,$low,$stageLevel);
	}else{
		$cp = getCrashPattern($stageNo,$low);
		$sp = getSpecialPattern($stageNo,$low);
		$mp = getMissilePattern($stageNo,$high,$stageLevel);
	}
	

	$p = array($cp,$sp,$mp);
	
	if($stageLevel>=2){
		$mp2 =getMissilePattern($stageNo+rand(1,100),$high,$stageLevel);
		$p[]=$mp2;
	}
	
	if(($sType==1 && $stageLevel>10) || $stageLevel>15){
		$sp2 = getSpecialPattern($stageNo+rand(1,100),$high);
		$p[]=$sp2;
	}

	if($sType==2 && $stageLevel>10){
		$cp2 = getCrashPattern($stageNo+rand(1,100),$high);
		$p[]=$cp2;
	}

	return $p;
}

function getCircleBoss($stageNo,$stageLevel){
	$boss= array();
	$BossNo = $stageNo%24+1;
	$level = (int)$stageLevel/2;
	if($level>10)$level=10;

	
	$boss["type"]="circle_".$BossNo;
	if($BossNo<10)$boss["type"]="circle_0".$BossNo;
	$boss["shape"]="circle";
	
	if($stageNo%5==0 && $stageNo>10){
		$BossNo = $stageNo%10+1;
		$boss["type"]="snake_".$BossNo;
		if($BossNo<10)$boss["type"]="snake_0".$BossNo;
		$boss["shape"]="snake";
	}
	
	$boss["scale"]=array("min"=>0.3,"start"=>0.4,"max"=>0.4); //

	$movement = rand(1,4);
	$boss["movement"]=array("normal"=>$movement,"draw"=>$movement); // 1~4
	
	//autoBalance로 조절되는 놈. level에 따라조절
	$boss["attackpercent"]= (0.3-0.2)/(float)10.0*$level+0.2; //0.1~0.4;
	$boss["ai"]=10*$level;
	
	//나머지
	//speed max 0.5~1.5 , min=max/2  
	$sType = rand(0,2);
	$high = 0.5*$stageLevel;
	$low = 0.25*$stageLevel;
	if($sType==0){ //hp형 보스
		$boss["hp"]=200+150*$stageNo;
		$boss["agi"]=5*$stageNo;
		$maxSpeed = (1.5-0.5)/(float)10.0*$low+0.5;
		if($maxSpeed>1.5)$maxSpeed=1.5;
		$boss["speed"]=array("min"=>$maxSpeed/(float)2,"start"=>$maxSpeed,"max"=>$maxSpeed);  
	}else if($sType=1){ //회피형 보스
		$boss["hp"]=200+100*$stageNo;
		$boss["agi"]=10*$stageNo;
		$maxSpeed = (1.5-0.5)/(float)10.0*$low+0.5;
		if($maxSpeed>1.5)$maxSpeed=1.5;
		$boss["speed"]=array("min"=>$maxSpeed/(float)2,"start"=>$maxSpeed,"max"=>$maxSpeed);  
	}else{ //speed형 보스
		$boss["hp"]=200+100*$stageNo;
		$boss["agi"]=5*$stageNo;
		$maxSpeed = (1.5-0.5)/(float)10.0*$high+0.5;
		if($maxSpeed>1.5)$maxSpeed=1.5;
		$boss["speed"]=array("min"=>$maxSpeed/(float)2,"start"=>$maxSpeed,"max"=>$maxSpeed);  
	}


	$pattern = getPattern($stageNo,$stageLevel+5);
	$boss["pattern"]=$pattern;
	return $boss;
	//$sankeBoss=$number%10+1;
}

function getJuniors($stageNo,$stageLevel,$boss){
	$juniors=array();
	$jCnt = $stageLevel/2;
	if($jCnt>10)$jCnt=10;
	if($jCnt<2)$jCnt=2;
	$movement = rand(1,4);
	$jrSpeed = $boss["speed"]["max"]/(float)2;
	$jrNo = $stageNo%10+1;
	$jrName = "jr_".$jrNo;
	if($jrNo<10)$jrName="jr_0".$jrNo;
	for($i=0;$i<$jCnt;$i++){
		$junior=array();
		$junior["type"] = $jrName;
		$junior["speed"] = array("min"=>$jrSpeed*0.5,"start"=>$jrSpeed,"max"=>$jrSpeed);
		$junior["scale"] = array("min"=>0.2,"start"=>0.35,"max"=>0.35);
		$junior["movement"] = array("normal"=>$movement,"draw"=>$movement);
		$junior["hp"] = (int)($boss["hp"]*0.2);
		$junior["agi"] = (int)($boss["agi"]*0.5);
		$junior["ai"] = (int)($boss["ai"]*0.5);

		$juniors[]=$junior;
	}
	return $juniors;
}




$bossData = array();
$bossData[0] = getCircleBoss(20,5);
echo json_encode($bossData,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

$juniors = getJuniors(20,5,$bossData[0]);
echo json_encode($juniors,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);


$result = mysql_query("select * from aStageTable where level>=1 and no<1000 order by no asc",DBManager::get()->getMainConnection());
while($data = mysql_fetch_array($result)){
	$data[level] = $data[level];

	$bossData = array();
	$bossData[0] = getCircleBoss($data[no],$data[level]);
	$juniors = getJuniors($data[no],$data[level],$bossData[0]);

	$bString = json_encode($bossData,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	$jString = json_encode($juniors,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

	$query = "update aStageTable set junior='".$jString."', boss='".$bString."', version=version+1 where no =".$data[no];
	mysql_query($query);
	echo mysql_error();
	echo $query."<br><br>";

	$cards = json_decode($data[cards],true); //$data[cards];
	$cardStat = getCardMissile($bossData[0],$data[level]);
		for($ci=0;$ci<count($cards);$ci++){
			$reward = $stageReward[$ci];
			$sString = json_encode($cardStat[$ci],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs1 = "update aCardTable set missile='".$sString."',stage=".$data[no]." where no=".$cards[$ci];
			mysql_query($qs1,DBManager::get()->getMainConnection());
			echo "<br><font color=blue>$qs1</font>";
			echo json_encode($cardStat,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			echo "<br><br>";

		}


}

/*
	조절할것
	패턴 = getPattern($stageNo,$stageLevel)
	몬스터속성 AI,SPEED,SCALE,MOVEMENT,ATTACKTURM,HP,DEX   AI,SPEED,ATTACKTURM은 서로 상대적, HP,DEX는 레벨선형적, SCALE는 고정, MOVEMENT는 랜덤
	쫄짜 COUNT, AI, SPEED, SCALE, MOVEMENT, HP, DEX  COUNT랜덤, 나머지는 보스따라가기
	

getCrashPattern($no,$level)
getSpecialPattern($no,$level)
getMissilePattern($no,$level)
getCircleBoss($no,$level,$stageLevel)

*/
?>











<?php
if(!$_GET[mode])exit;





exit; 



if(!$_GET[mode])exit;


//스테이지번호 1~순서대로 가기
// $i=1;
// $levels = array(0,5,6,7,8,9,10,10,10,10,10,10,10,10,10,10,10); 
// $c=1;
// $query = mysql_query("select * from aStageTable order by no asc",DBManager::get()->getMainConnection());
// while($data = mysql_fetch_array($query)){
// 	$n = $data[no];

// 	$levels[$c]=$levels[$c]-1;
// 	if($levels[$c]<0)$c++;
	
// 	$l = $c;
// 	echo $i."<br>";
// 	mysql_query("update aStageTable set no=".$data[num]." where num=".$data[num],DBManager::get()->getMainConnection());
// 	$i++;
// }



return;



?>


<table width=1024><tr><td>


<?



//퍼즐별 카드번호 자동세팅하기 
/*
$puzzleNo = 6;
$cardHeader = 11;

$result = mysql_query("select * from $TABLE_STAGE where puzzle=$puzzleNo",DBManager::get()->getMainConnection());
echo mysql_error();
$i=0;
while($stageInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
	
	echo "abc";
	$zero = "0";
	if($i>10)$zero="";
	$mycard = $cardHeader*1000+$i*10;
	$cardtext = "[".($mycard+1).",".($mycard+2).",".($mycard+3)."]";
	//echo("update $TABLE_STAGE set cards = '".$cardtext."' where no=".$stageInfo[no]);
	mysql_query("update $TABLE_STAGE set cards = '".$cardtext."' where no=".$stageInfo[no],DBManager::get()->getMainConnection());

	echo "<br>";
	$i++;
}
*/
//카드파일명으로 카드자동생성하기

// $startCardNo = 11000;

// $oldCardID=0;
// $realDir = "../images/cards";
// $i=0;
// $t=($startCardNo/1000)-1;
// $files = filesInDir ($realDir);

// $fileText = "/Volumes/PublicHDD/Galsprototype/c_cf_01_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_3.png";

// $files = explode("\n",$fileText);

// sort($files);
// foreach ( $files as $key => $a_file){ 

// 	$filepath = explode("/",$a_file);
// 	$filename = $filepath[count($filepath)-1];
// 	$filename = explode(".",$filename);
// 	$filename = $filename[0];

// 	$fileinfo = explode("_",$filename);
// 	$type = $fileinfo[1];
// 	$typeNo = $fileinfo[2];
// 	$rank = $fileinfo[3];
// 	$isSil = true;
// 	if(is_numeric($rank))$isSil=false;

// 	$newCardID = $type.$typeNo;
// 	$newTheme = $type;


// 	if($i!=0 && $oldCardID==$newCardID){
// 		$oldCardID = $newCardID;
// 		continue;
// 	}


// 	if($newTheme!=$oldTheme)$t++;
// 	$i++;

// 	$filepath_head = "../images/";
// 	$filename_head = "cards/"."c_".$type."_".$typeNo."_";

// 	$card1 = $filename_head."1.png";
// 	$card1s = $filename_head."1s.png";
// 	$card2 = $filename_head."2.png";
// 	$card2s = $filename_head."2s.png";
// 	$card3 = $filename_head."3.png";
// 	$cardNo = ($typeNo-1)*10+1000*$t;
	
// 	$data1=array();
// 	$data1[no]=$cardNo+1;
// 	$data1[rank]=1;
// 	$data1[reward]=500;
// 	$data1[durability]=5;
// 	$data1[language]="kr";
// 	$data1[imgInfo]='{"img":"'.$card1.'","size":'.(int)(filesize($filepath_head.$card1)/1000).'}';
// 	$data1[silImgInfo]='{"isSil":"1","img":"'.$card1s.'","size":'.(int)(filesize($filepath_head.$card1s)/1000).'}';

// 	$data2=array();
// 	$data2[no]=$cardNo+2;
// 	$data2[rank]=2;
// 	$data2[reward]=1000;
// 	$data2[durability]=5;
// 	$data2[language]="kr";
// 	$data2[imgInfo]='{"img":"'.$card2.'","size":'.(int)(filesize($filepath_head.$card2)/1000).'}';
// 	$data2[silImgInfo]='{"isSil":"1","img":"'.$card2s.'","size":'.(int)(filesize($filepath_head.$card2s)/1000).'}';

// 	$data3=array();
// 	$data3[no]=$cardNo+3;
// 	$data3[rank]=3;
// 	$data3[reward]=2000;
// 	$data3[durability]=5;
// 	$data3[language]="kr";
// 	$data3[imgInfo]='{"img":"'.$card3.'","size":'.(int)(filesize($filepath_head.$card3)/1000).'}';
// 	$data3[silImgInfo]='{"isSil":"0"}';
	
// 	$query1 = lq_query_insert($data1,$TABLE_CARD);
// 	$query2 = lq_query_insert($data2,$TABLE_CARD);
// 	$query3 = lq_query_insert($data3,$TABLE_CARD);

// 	mysql_query($query1,DBManager::get()->getMainConnection());
// 	mysql_query($query2,DBManager::get()->getMainConnection());
// 	mysql_query($query3,DBManager::get()->getMainConnection());

// 	echo $query1;
// 	echo $query2;
// 	echo $query3;
	
// 	echo $t."->".$i."->".$cardNo."->".$filename."->".$newCardID."<br>"; 

// 	$oldCardID = $newCardID;
// 	$oldTheme=$type;
// } 


/*

//몬스터 세팅~




$res["boss_metal1"]["circle"][]="circle_01";
$res["boss_metal1"]["circle"][]="circle_02";
$res["boss_metal1"]["circle"][]="circle_03";
$res["boss_metal1"]["circle"][]="circle_04";
$res["boss_metal1"]["circle"][]="circle_05";
$res["boss_metal1"]["circle"][]="circle_06";
$res["boss_metal1"]["circle"][]="circle_07";
$res["boss_metal1"]["circle"][]="circle_08";
$res["boss_metal1"]["circle"][]="circle_09";
$res["boss_metal1"]["circle"][]="circle_10";
$res["boss_metal1"]["circle"][]="circle_11";
$res["boss_metal1"]["circle"][]="circle_12";
$res["boss_metal1"]["circle"][]="circle_13";




$res["boss_metal2"]["circle"][]="circle_14";
$res["boss_metal2"]["circle"][]="circle_15";
$res["boss_metal2"]["circle"][]="circle_16";
$res["boss_metal2"]["circle"][]="circle_17";
$res["boss_metal2"]["circle"][]="circle_18";
$res["boss_metal2"]["circle"][]="circle_19";
$res["boss_metal2"]["circle"][]="circle_20";
$res["boss_metal2"]["circle"][]="circle_21";
$res["boss_metal2"]["circle"][]="circle_22";
$res["boss_metal2"]["circle"][]="circle_23";
$res["boss_metal2"]["circle"][]="circle_24";

$res["boss_metal1"]["jr"][]="jr_01";
$res["boss_metal1"]["jr"][]="jr_02";
$res["boss_metal1"]["jr"][]="jr_03";
$res["boss_metal1"]["jr"][]="jr_04";
$res["boss_metal1"]["jr"][]="jr_05";
$res["boss_metal1"]["jr"][]="jr_06";
$res["boss_metal1"]["jr"][]="jr_07";
$res["boss_metal1"]["jr"][]="jr_08";



$res["boss_metal2"]["jr"][]="jr_09";
$res["boss_metal2"]["jr"][]="jr_10";

$res["boss_metal1"]["snake"][]="snake_01";
$res["boss_metal1"]["snake"][]="snake_02";
$res["boss_metal1"]["snake"][]="snake_03";
$res["boss_metal1"]["snake"][]="snake_04";
$res["boss_metal1"]["snake"][]="snake_05";


$res["boss_metal2"]["snake"][]="snake_06";
$res["boss_metal2"]["snake"][]="snake_07";
$res["boss_metal2"]["snake"][]="snake_08";
$res["boss_metal2"]["snake"][]="snake_09";
$res["boss_metal2"]["snake"][]="snake_10";

foreach ($res as $plistName => $value){
	foreach ($value as $mobType => $filelist) {
		foreach ($filelist as $no => $filename){
			echo $plistName."->".$mobType."->".$filename."<br>";
			$mobinfo["ccbiID"]=$filename;
			$mobinfo["ccbi"]="resource/".$filename;
			$mobinfo["imageID"]=$plistName;
			$mobinfo["plist"]="resource/".$plistName.".plist";
			$mobinfo["pvrccz"]="resource/".$plistName.".pvr.ccz";
			$mobinfo["size"]=(int)(filesize("../".$mobinfo["pvrccz"])/1000);
			$mobinfo["type"]=$mobType;
			$rInfo =  json_encode($mobinfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK); 
			echo("insert into $TABLE_MONSTER ('name','resourceInfo','type') values ('".$filename."','".$rInfo."','".$mobType."') <br>");
			mysql_query("insert into $TABLE_MONSTER (`name`,`resourceInfo`,`type`) values ('".$filename."','".$rInfo."','".$mobType."')",DBManager::get()->getMainConnection());
			echo mysql_error();
			echo "ok<br><br>";
		}
	}
}
*/
DBManager::get()->closeDB();

?>

</td></tr></table>

