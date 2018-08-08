$(document).ready(function(){
  $.get("ajax-page", function(data, status) {
    alert("Data"+data+", status: "+status);
  });
});
