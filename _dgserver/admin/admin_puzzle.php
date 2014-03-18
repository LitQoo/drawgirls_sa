<?php
include "header.php";
?>
<script>
var showPuzzleImg = function(value,option){
	data = s2j(value);
	if(!data)return "";
	if(!data["image"]) return "";
	return '<img src=../images/'+data["image"]+' width=300>';
}
</script>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_PUZZLE?>" dbSort="`order` asc" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="order" viewer="text" editor="text">order</th>
			<th field="isEvent" viewer="text" editor="select" editorOption="[0,1,2]">isEvent <br> (0:일반,1:이벤트,2:미노출)</th>
			<th field="title" viewer="text" editor="text">title</th>
			<th field="thumbnail" viewer="custom" viewerOption="showPuzzleImg" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>thumbnail</th>
			<!--th field="top" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>top</th>
			<th field="bottom" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>bottom</th>
			<th field="left" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>left</th>
			
			<th field="right" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>right</th-->
			<th field="center" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>center</th>
			<th field="original" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>original</th>
			<th field="face" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>face</th>
			<th field="map" viewer="custom" viewerOption="showPuzzleImg" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>map</th>
			<th field="coordinateInfo"  viewer="json" editor="table" editorOption='[
			{"title":"x","field":"x","editor":"text","editorOption":{"datatype":"int"}}	,
			{"title":"y","field":"y","editor":"text","editorOption":{"datatype":"int"}}					
			]'>coordinateInfo</th>

			<th field="startPosition" viewer="text" editor="dictionary" editorOption='
			[
				{"field":"x","editor":"text","editorOption":{"datatype":"int"}},
				{"field":"y","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>startPosition</th>

			<th field="endPosition" viewer="text" editor="dictionary" editorOption='
			[
				{"field":"x","editor":"text","editorOption":{"datatype":"int"}},
				{"field":"y","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>endPosition</th>
			<th field="point" viewer="text" editor="text" editorOption='{"datatype":"int"}'>point</th>
			<th field="pathInfo" viewer="json" editor="array" editorOption='{"title":"피스번호","field":"type","editor":"text","editorOption":{"datatype":"int"}}'>pathInfo</th>
			<th field="cardInfo" viewer="json" editor="array" editorOption='{"title":"카드목록","field":"type","editor":"array","editorOption":{"title":"카드번호","field":"type","editor":"text","editorOption":{"datatype":"int"}}}'>cardInfo</th>
			<th field="rewardInfo" viewer="json">rewardInfo (자동갱신)</th>
			<th field="levelInfo" viewer="json">levelInfo (자동갱신)</th>
			<th field="conditionInfo" viewer="json">conditionInfo (자동갱신)</th>
			<th field="ticket" viewer="text" editor="text" editorOption='{"datatype":"int"}'>ticket</th>
			<th field="point" viewer="text" editor="text" editorOption='{"datatype":"int"}'>point</th>
			<th field="version" viewer="text" editor="text" editorOption='{"datatype":"int"}'>version</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>