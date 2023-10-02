<?php
include("connect.php");

$link = Connection();

// Sélectionner les données de la table "information_table"
$query = "SELECT * FROM `information_table` ORDER BY `id` DESC";

$result = mysqli_query($link, $query);

?>

<html>
   <head>
      <title>Données des capteurs</title>
   </head>
<body>
   <h1>Données des capteurs</h1>

   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td>&nbsp;ID&nbsp;</td>
			<td>&nbsp;Carte ID&nbsp;</td>
			<td>&nbsp;Solde&nbsp;</td>
			<td>&nbsp;Carte Code&nbsp;</td>
		</tr>

      <?php 
		  if ($result !== FALSE) {
		     while ($row = mysqli_fetch_assoc($result)) {
		        printf(
		            "<tr><td> &nbsp;%s </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td></tr>",
		            $row["id"], $row["carte_id"], $row["solde"], $row["carte_code"]
		        );
		     }
		     mysqli_free_result($result);
		     mysqli_close($link);
		  }
      ?>

   </table>
</body>
</html>
