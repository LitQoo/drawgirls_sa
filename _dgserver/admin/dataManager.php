<?php
include "../lib.php";		


$mode = $_POST['mode'];
$primaryKey = $_POST['primaryKey'];
$primaryValue = $_POST['primaryValue'];
$table = $_POST['table'];
$data = $_POST['data'];
$sort = $_POST['sort'];
$where = $_POST['where'];
$limit = $_POST['limit'];

if(!$table)$table=$_GET['table'];
/*
		$data = json_decode(stripslashes($data));	
	if($table==$TABLE_STAGE){
		$data["version"]=$data["version"]+1;
		kvManager::increase("stageVersion_".$data[no]);
		kvManager::increase("puzzleVersion_".$data[puzzle]);
	}
	if($table==$TABLE_PUZZLE){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVersion_".$data[no]);
	}
	
*/

	if($data)$data = json_decode($data,true);	
	
	if($table==$TABLE_STAGE && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("stageVer_".$data[no]);
		$pData = mysql_fetch_assoc(mysql_query("select * from $TABLE_PUZZLE where no=$data[puzzle]",DBManager::get()->getMainConnection()));
		kvManager::increase("puzzleVer_".$pData[no]);
		kvManager::increase("puzzleListVer");

		mysql_query("update $TABLE_PUZZLE set version=version+1 where no=".$data[puzzle],DBManager::get()->getMainConnection());

		$cardlist = json_decode($data["cards"],true);

		for($i=0;$i<count($cardlist);$i++){
			mysql_query("update $TABLE_CARD set `stage`=".$data[no].", `grade`=".($i+1)." where no=".$cardlist[$i],DBManager::get()->getMainConnection());
		}
	}
	if($table==$TABLE_EVENTSTAGE && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("eStageVer_".$data[no]);
		kvManager::increase("eStageListVer");
	}
	if($table==$TABLE_PUZZLE && $data["isEvent"]==1){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVer_".$data[no]);
		kvManager::increase("eventPuzzleListVer");
		mysql_query("update $TABLE_PUZZLE set version=version+1 where no=".$data[no],DBManager::get()->getMainConnection());

	}else if($table==$TABLE_PUZZLE && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVer_".$data[no]);
		kvManager::increase("puzzleListVer");
	}
	if($table==$TABLE_CARD && $data){
		kvManager::increase("cardVer_".$data[no]);
		kvManager::increase("stageVer_".$data[stage]);
	}
	if($table==$TABLE_CHARACTER && $data){
		kvManager::increase("charListVer");
	}
	if($table==$TABLE_MONSTER && $data){
		kvManager::increase("monsterListVer");
	}

	if($table==$TABLE_SHOP && $data){
		kvManager::increase("shopListVer");
	}
	
if($mode=="update"){
	if(!$primaryKey || !$primaryValue || !$table){ echo'{"result":"fail"}'; mysql_close(); exit; }
	if($table==DBManager::getST("weeklyscore")){
		$dataObj = new WeeklyScore($data["memberID"],$data["weekNo"]);
		$dataObj->m_memberID=$data["memberID"];
		$dataObj->m_regWeek=$data["regWeek"];
		$dataObj->m_score=$data["score"];
		$dataObj->m_regDate=$data["regDate"];
		$dataObj->m_data=$data["data"];
		$result=$dataObj->save();
		$data[log] = LogManager::get()->getLogAndClear();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail"}';

	}else if($table==DBManager::getST("stagescore")){
		$dataObj = new StageScore($data["stageNo"],$data["memberID"]);
		LogManager::get()->addLog("modify stagescore");
		LogManager::get()->addLog("modify stagescore is load? ".$dataObj->isLoaded());


		$dataObj->m_memberID=$data["memberID"];
		$dataObj->m_stageNo=$data["stageNo"];
		$dataObj->m_score=$data["score"];
		$dataObj->m_regDate=$data["regDate"];
		$dataObj->m_data=$data["data"];
		$result=$dataObj->save();
		$data[log] = LogManager::get()->getLogAndClear();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail"}';

	}else if($table==DBManager::getST("message")){
		//보내기
		$message = new Message($data["memberID"],$data["no"]);
		$message->m_memberID=$data["memberID"];
		$message->m_content=$data["content"];
		$message->m_regDate=$data["regDate"];
		$message->m_friendID=$data["friendID"];
		$message->m_type=$data["type"];
		$message->m_isSendMsg=$data["isSendMsg"];
		$result=$message->save();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail"}';

	}else if($table==DBManager::getST("userdata")){
		//보내기
		$userdata = new UserData($data["memberID"]);
		$userdata->m_data = $data["data"];
		$userdata->m_friendList = $data["friendList"];

		$result=$userdata->save();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail"}';

	}else{
	
		$query = lq_query_update($data,$table,"where $primaryKey = $primaryValue");
		

		if(mysql_query($query,DBManager::get()->getMainConnection())){
			//"query":"'.addslashes($query).'"
			echo'{"result":"ok","data":'.json_encode($data).'}'; mysql_close(); exit; 
		}
		
		echo'{"result":"fail","query":"'.addslashes($query).'"}'; mysql_close(); exit; 

	}
}else if($mode=="delete"){
	if(!$primaryKey || !$primaryValue || !$table){ echo'{"result":"fail"}'; mysql_close(); exit; }

	if($table==DBManager::getST("message") || $table==DBManager::getST("userdata")){
		//보내기
		// $message = new Message($data["memberID"],$data["no"]);
		// $result = $message->remove();
		// if($result)echo'{"result":"ok"}';
		// else 

		echo'{"result":"fail"}';
	}else{
		$query = "delete from $table where $primaryKey = $primaryValue";
		if(mysql_query($query,DBManager::get()->getMainConnection())){
			//,"query":"'.addslashes($query).'"
			echo'{"result":"ok"}'; mysql_close(); exit; 
		}
		echo'{"result":"fail","query":"'.addslashes($query).'"}';mysql_close(); exit; 
	}
}else if($mode=="insert"){
	//$data = json_decode($data,true);
	


	if(!$table){ echo'{"result":"fail"}'; mysql_close();exit; }
	

	if($table==DBManager::getST("message")){
		//보내기
		$message = new Message();
		$message->m_memberID=$data["memberID"];
		$message->m_content=$data["content"];
		$message->m_regDate=TimeManager::get()->getCurrentDateString();
		$message->m_friendID=$data["friendID"];
		$message->m_type=$data["type"];
		$message->m_isSendMsg=$data["isSendMsg"];
		$r["send"]=$message->send();
		echo'{"result":"ok"}';
	}else{

		$query = lq_query_insert($data,$table);

		if(mysql_query($query,DBManager::get()->getMainConnection())){
			$no = mysql_insert_id(DBManager::get()->getMainConnection());
			$primaryString="";
			if($no)$primaryString=',"'.$primaryKey.'":'.$no;
			echo'{"result":"ok" '.$primaryString.'}'; mysql_close();exit; 
		}


		echo'{"result":"fail","query":"'.addslashes($query).'","data":'.json_encode($data).'}'; mysql_close();exit; 
	}

	
}else{
	if(!$table){ echo'{"result":"fail"}'; @mysql_close();exit; }

	if($table==DBManager::getST("stagescore")){
		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    //testtest
	    while($userdata = StageScore::getRowByQuery($qstr)){
			$memberID = $userdata["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userdata["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userdata["nick"]=$userInfo->m_nick;

			$dataList[]=$userdata;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		$resort = "regDate";
		if($sort){
			$resort = explode(" ",$sort);
			$resort = $resort[0];
		}
		usort($dataList, build_sorter($resort));

		rsort($dataList); 

	    echo json_encode($dataList);
	}elseif($table==DBManager::getST("weeklyscore")){
		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    //testtest
	    while($userdata = WeeklyScore::getRowByQuery($qstr)){
			$memberID = $userdata["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userdata["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userdata["nick"]=$userInfo->m_nick;

			$dataList[]=$userdata;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		$resort = "regDate";
		if($sort){
			$resort = explode(" ",$sort);
			$resort = $resort[0];
		}
		usort($dataList, build_sorter($resort));

		rsort($dataList); 

	    echo json_encode($dataList);
	}else if($table==DBManager::getST("message")){
		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    while($data = Message::getRowByQuery($qstr)){
			$memberID = $data["memberID"];
			$friendID = $data["friendID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($data["memberID"]);
			}
			if(!$userList[$friendID]){
				$userList[$friendID] = new UserData($data["friendID"]);
			}

			$userInfo = $userList[$memberID];
			$friendInfo = $userList[$friendID];
			$data["nick"]=$userInfo->m_nick;
			$data["friendNick"]=$friendInfo->m_nick;

			$dataList[]=$data;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($dataList, build_sorter('regDate'));

		rsort($dataList); 

	    echo json_encode($dataList);
	}else if($table==DBManager::getST("userlog")){

		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$logList = array();
	    $userList = array();
	    while($userlog = UserLog::getRowByQuery($qstr)){
			$memberID = $userlog["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userlog["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userlog["nick"]=$userInfo->m_nick;

			$logList[]=$userlog;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($logList, build_sorter('regDate'));

		rsort($logList); 

	    echo json_encode($logList);

	}else if($table == DBManager::getST("userdata")){

		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    //testtest
	    while($userdata = UserData::getRowByQuery($qstr)){
			$memberID = $userdata["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userdata["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userdata["nick"]=$userInfo->m_nick;

			$dataList[]=$userdata;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($dataList, build_sorter('regDate'));

		rsort($dataList); 

	    echo json_encode($dataList);

	}else{
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$query = @mysql_query("select * from `$table` ".$qstr,DBManager::get()->getMainConnection());
		echo"[";
		$isFirst = true;
		while($data = @mysql_fetch_array($query,MYSQL_ASSOC)){
			
			if(!$isFirst){
				echo ",";
			}
			$isFirst=false;
			echo json_encode($data);	
		}
	echo"]";
	}
}


DBManager::get()->closeDB();

?>

