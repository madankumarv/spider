<?php
   if(isset($_FILES['image'])){
      $errors= array();
      $file_name = $_FILES['image']['name'];
      $file_size = $_FILES['image']['size'];
      $file_tmp = $_FILES['image']['tmp_name'];
      $file_type = $_FILES['image']['type'];
      $file_ext = substr($file_name, (strpos($file_name,'.')+1));
      
      $expensions= array("jpeg","jpg","png","txt");
      
      if(in_array($file_ext,$expensions)=== false){
         echo $file_ext;
         $errors[]="extension not allowed, please choose a JPEG or PNG file.";
      }
      
      if($file_size > 2097152) {
         $errors[]='File size must be excately 2 MB';
      }
      
      if(empty($errors)==true) {
         move_uploaded_file($file_tmp,"files/".$_POST['library']."/".$file_name);
         echo "Success";
      }else{
         print_r($errors);
      }
   }
?>
<html>
   <body>
      <?php 
        $dir = "./files";
        $list = scandir($dir);
        $subdirs = "";
        foreach($list as $name) {
          if(strpos($name, '.') === false) {
            $subdirs .= "<option>$name</option>";
          }
        }
      ?>
      <form action = "" method = "POST" enctype = "multipart/form-data">
        Library:
        <select name="library" id="library">
          <?php echo $subdirs ?>
        </select>
         <input type = "file" name = "image" />
         <input type = "submit"/>
      </form>
      
   </body>
</html>
