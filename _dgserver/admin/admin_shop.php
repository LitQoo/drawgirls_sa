<?php
include "header.php";

?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_SHOP?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" primary>no</th>
			<th field="type" viewer="text" editor="select" editorOption='["gold","ruby","social","coin"]'>type</th>
			<th field="count" viewer="text" editor="text" editorOption='{"datatype":"int"}'>count</th>
			<th field="price" viewer="text" editor="text" editorOption='{"datatype":"int"}'>price</th>
			<th field="priceType" viewer="text" editor="select" editorOption='["gold","ruby","social","coin","money"]'>priceType</th>
			<th field="sale" viewer="text" editor="text" editorOption='{"datatype":"int"}'>sale</th>

			<th field="image" viewer="text" editor="text">image</th>
			<th field="data" viewer="json" editor="dictionary" editorOption=''>data</th>

			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";
?>