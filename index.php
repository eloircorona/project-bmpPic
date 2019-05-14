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

$opt_names = array(
    '0' => 'Separar color',
    '1' => 'Monocromatico',
    '2' => 'Blur',
    '3' => 'Espejo',
    '4' => 'Bordes',
    '5' => 'Movimiento'
);

$opt_name = '';
switch ($_SESSION['opt']) {
    case 1:
        $opt_name = $opt_names[0];
        break;
    case 2:
        $opt_name = $opt_names[1];
        break;
    case 3:
        $opt_name = $opt_names[2];
        break;
    case 4:
        $opt_name = $opt_names[3];
        break;
    case 5:
        $opt_name = $opt_names[4];
        break;
    case 6:
        $opt_name = $opt_names[5];
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
    'opt_names' => $opt_names,
    'image_processed' => $BASE_URL . 'assets/processed/' . $_SESSION['image']
);


// Upload image
$upload = -1;
if(isset($_POST['submit']))
{
    $_SESSION['image'] = $_FILES["file"]["name"];

    $path = 'assets/uploads/' . basename($_FILES["file"]["name"]);
    $imageFileType = strtolower(pathinfo($path, PATHINFO_EXTENSION));
    if (file_exists($path)) $upload = 0;
    if($imageFileType != "bmp") $upload = 2;

    if($upload == 0)
    {
        $_SESSION['output'] = "Ya existe el archivo";
    }
    elseif($upload == 1)
    {
        if (move_uploaded_file($_FILES["file"]["tmp_name"], $path)) {
            $_SESSION['output'] = 'Archivo subido exitosamente, ' . execute($path, 1, 255, 0, 0);
        } else {
            $_SESSION['output'] = 'Error al subir el archivo';
        }
    }
    elseif($upload == 2)
    {
        $_SESSION['output'] = "El archivo no está en formato BMP";
    }
    $data['menu']['image_processed'] = $BASE_URL . 'assets/processed/' . $_SESSION['image'];
} else $_SESSION['output'] = 'Favor de subir una imagen';
$data['menu']['output'] = $_SESSION['output'];

if($upload == 1) header('Location: ' . $BASE_URL . 'index.php?opt=1&image=' . $data['menu']['image']);

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
