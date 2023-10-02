<?php

function Connection(){
    $server = "localhost";
    $user = "id21292157_projete2user";
    $pass = "projet-E2-password";
    $db = "id21292157_projete2database";

    // Connexion à la base de données avec MySQLi
    $connection = new mysqli($server, $user, $pass, $db);

    // Vérifier la connexion
    if ($connection->connect_error) {
        die('MySQLi ERROR: ' . $connection->connect_error);
    }

    // Sélectionner la base de données
    if (!$connection->select_db($db)) {
        die('MySQLi ERROR: Impossible de sélectionner la base de données ' . $db);
    }

    return $connection;
}
?>
