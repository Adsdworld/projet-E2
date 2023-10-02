<?php
include("connect.php");

$link = Connection();

$carte_id = $_POST["carte_id"];
$solde = $_POST["solde"];
$carte_code = $_POST["carte_code"];

// Préparez la requête SQL en utilisant MySQLi avec des déclarations préparées pour éviter les injections SQL
$query = "INSERT INTO `information_table` (`carte_id`, `solde`, `carte_code`) VALUES (?, ?, ?)";
$stmt = mysqli_prepare($link, $query);

if ($stmt) {
    // Associez les valeurs aux paramètres de la requête
    mysqli_stmt_bind_param($stmt, "sss", $carte_id, $solde, $carte_code); //iss pour un entier 2 string 

    // Exécutez la requête
    mysqli_stmt_execute($stmt);

    // Fermez la déclaration préparée
    mysqli_stmt_close($stmt);
} else {
    echo "Erreur de préparation de la requête: " . mysqli_error($link);
}

mysqli_close($link);
?>
