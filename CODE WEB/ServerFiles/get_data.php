<?php
include("connect.php");

$link = Connection();

if (isset($_GET["carte_id"])) {
    $carte_id = $_GET["carte_id"];
    
    $query = "SELECT * FROM `information_table` WHERE `carte_id` = '$carte_id'";
    $result = mysqli_query($link, $query);

    if ($result !== FALSE) {
        $data = mysqli_fetch_assoc($result);
        echo json_encode($data);
    } else {
        echo "Erreur de requête SQL : " . mysqli_error($link);
    }

    mysqli_close($link);
} else {
    echo "Paramètre 'carte_id' manquant.";
}
?>
