<?php
session_start();
require_once './vendor/autoload.php';

$loader = new Twig_Loader_Filesystem('./interface');
$twig = new Twig_Environment($loader, []);

echo $twig->render('header.twig');

$BASE_URL = "http://localhost/project-bmpPic/";

if(!isset($_SESSION['image'])) $_SESSION['image'] = '';
if(!isset($_SESSION['opt'])) $_SESSION['opt'] = 0;

if(isset($_GET['clear']))
{
    session_destroy();
    header('Location: index.php');
}

if(isset($_GET['image'])) $_SESSION['image'] = $_GET['image'];
if(isset($_GET['opt'])) $_SESSION['opt'] = $_GET['opt'];

$opt_name_1 = 'Separar color';
$opt_name_2 = 'Monocromatico';
$opt_name_3 = 'Blur';
$opt_name_4 = 'Espejo';
$opt_name_5 = 'Bordes';
$opt_name_6 = 'Movimiento';

$opt_name = '';
switch ($_SESSION['opt']) {
    case 1:
        $opt_name = $opt_name_1;
        break;
    case 2:
        $opt_name = $opt_name_2;
        break;
    case 3:
        $opt_name = $opt_name_3;
        break;
    case 4:
        $opt_name = $opt_name_4;
        break;
    case 5:
        $opt_name = $opt_name_5;
        break;
    case 6:
        $opt_name = $opt_name_6;
        break;

    default:
        $opt_name = 'Sin filtro';
        break;
}

// Variable for templates
$data['menu'] = array(
    'base_url' => $BASE_URL,
    'opt' => $_SESSION['opt'],
    'image' => $_SESSION['image'],
    'image_name' => $BASE_URL . 'assets/uploads/' . $_SESSION['image'],
    'opt_name' => $opt_name,
    'opt_name_1' => $opt_name_1,
    'opt_name_2' => $opt_name_2,
    'opt_name_3' => $opt_name_3,
    'opt_name_4' => $opt_name_4,
    'opt_name_5' => $opt_name_5,
    'opt_name_6' => $opt_name_6,
    'image_processed' => $BASE_URL . 'assets/processed/' . $_SESSION['image']
);


// Upload image
$upload = -1;
if(isset($_POST['submit']))
{
    $_SESSION['image'] = $_FILES["file"]["name"];

    $path = 'assets/uploads/' . basename($_FILES["file"]["name"]);
    $_SESSION['path'] = $path;

    $imageFileType = strtolower(pathinfo($path, PATHINFO_EXTENSION));
    if (file_exists($path)) $upload = 0;
    if($imageFileType != "bmp") $upload = 2;

    if($upload == 0)
    {
        $_SESSION['output'] = "Ya existe el archivo: " . $path;
    } else {
            $upload = 1;
    }

    if($upload == 1)
    {
        if (move_uploaded_file($_FILES["file"]["tmp_name"], $path)) {
            $_SESSION['output'] = 'Archivo subido exitosamente: ' . $path;
        } else {
            $_SESSION['output'] = 'Error al subir el archivo';
        }
    }
    elseif($upload == 2)
    {
        $_SESSION['output'] = "El archivo no está en formato BMP";
    }
    $data['menu']['image_processed'] = $BASE_URL . 'assets/processed/' . $_SESSION['image'];
}

if(isset($_SESSION['path']))
{
    execute($_SESSION['path'], $_SESSION['opt'], 255, 0, 0);
} else {
    $_SESSION['output'] = 'Favor de subir una imagen';
}
$data['menu']['output'] = $_SESSION['output'];

if($upload == 1) header('Location: ' . $BASE_URL . 'index.php?opt=1&image=' . $_SESSION['image']);

// Render template
echo $twig->render('menu.twig', $data['menu']);

//Render template
echo $twig->render('footer.twig');

// Function to run C program
function execute($path, $option, $parameter1, $parameter2, $parameter3)
{
    $command = 'project.exe ' . $option . ' ' . '"' . $path . '" ' . $parameter1 . ' ' . $parameter2 . ' ' . $parameter3;
    $result = shell_exec($command);
    return $result;
}


?>
