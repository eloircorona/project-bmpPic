<?php
session_start();
require_once './vendor/autoload.php';

$loader = new Twig_Loader_Filesystem('./interface');
$twig = new Twig_Environment($loader, []);

echo $twig->render('header.twig');

$BASE_URL = "http://localhost/project-bmpPic/";

if(isset($_GET['image'])) $_SESSION['image'] = $_GET['image'];
if(isset($_GET['opt'])) $_SESSION['opt'] = $_GET['opt'];

$data['menu'] = array(
    'base_url' => $BASE_URL,
    'opt' => $_SESSION['opt'],
    'image' => $_SESSION['image'],
    'image_name' => $BASE_URL . 'assets/uploads/' . $_SESSION['image']
);

echo $twig->render('menu.twig', $data['menu']);


echo $twig->render('footer.twig');
?>
