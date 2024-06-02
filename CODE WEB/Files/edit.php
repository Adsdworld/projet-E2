<?php
include("connect.php");

$link = Connection();

if (isset($_POST["id"])) {
    $id = $_POST["id"];
    $carte_id = $_POST["carte_id"];
    $solde = $_POST["solde"];
    $carte_code = $_POST["carte_code"];

    // Préparez la requête SQL en utilisant MySQLi avec des déclarations préparées pour éviter les injections SQL
    $query = "UPDATE `information_table` SET `I` = ?, `S` = ?, `C` = ? WHERE `A` = ?";
    $stmt = mysqli_prepare($link, $query);

    if ($stmt) {
        // Associez les valeurs aux paramètres de la requête
        mysqli_stmt_bind_param($stmt, "ssss", $carte_id, $solde, $carte_code, $id);

        // Exécutez la requête
        mysqli_stmt_execute($stmt);

        // Fermez la déclaration préparée
        mysqli_stmt_close($stmt);

        echo "Modification réussie.";
    } else {
        echo "Erreur de préparation de la requête: " . mysqli_error($link);
    }
    
    mysqli_close($link);
} else {
    echo "Paramètre 'id' manquant.";
}
?>
