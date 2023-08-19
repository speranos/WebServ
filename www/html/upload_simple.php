<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // var_dump($_FILES);
    // var_dump($_GET);
    $file = $_FILES['file'];
    $fileName = $file['name'];
    $fileTmpName = $file['tmp_name'];
    $fileSize = $file['size'];
    $fileError = $file['error'];
    
    var_dump($_FILES);
    if ($fileError === 0) {
        $destination = 'uploads/' . $fileName;
        move_uploaded_file($fileTmpName, $destination);
        echo "File uploaded successfully!";
    } else {
        echo "Error uploading file. Error code: $fileError";
    }
} else {
    ?>
    <form method="POST" enctype="multipart/form-data">
        <input type="file" name="file" required>
        <input type="submit" value="Upload">
    </form>
    <?php
}
?>