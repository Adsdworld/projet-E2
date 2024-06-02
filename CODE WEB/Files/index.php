<?php
include("connect.php");

$link = Connection();

// Sélectionner les données de la table "information_table"
$query = "SELECT * FROM `information_table` ORDER BY `A` DESC";

$result = mysqli_query($link, $query);

?>

<html>
   <head>
      <title>Base de Données</title>
   </head>
<body>
   <h1>Base de Données</h1>

   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td>&nbsp;ID&nbsp;</td>
			<td>&nbsp;Carte ID&nbsp;</td>
			<td>&nbsp;Solde&nbsp;</td>
			<td>&nbsp;Carte Code&nbsp;</td>
			<td>&nbsp;Prénom&nbsp;</td>
		</tr>

      <?php 
		  if ($result !== FALSE) {
		     while ($row = mysqli_fetch_assoc($result)) {
		        printf(
		            "<tr><td> &nbsp;%s </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td></tr>",
		            $row["A"], $row["I"], $row["S"], $row["C"], $row["P"]
		        );
		     }
		     mysqli_free_result($result);
		     mysqli_close($link);
		  }
      ?>

   </table>
</body>
</html>
