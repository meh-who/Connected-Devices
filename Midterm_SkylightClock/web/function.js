function showTime(){
  var date = new Date();
  var h = date.getHours(); 
  var m = date.getMinutes(); 
  var s = date.getSeconds(); 
  // （如果要显示秒针umcommon这个）
  var session = "AM";
  
  if(h == 0){
      h = 12;
  }
  
  if(h > 12){
      h = h - 12;
      session = "PM";
  }
  
  h = (h < 10) ? "0" + h : h;
  // （arduino使用时针数据）
  m = (m < 10) ? "0" + m : m;
  // （arduino使用分针数据）
  s = (s < 10) ? "0" + s : s;
  // （如果要显示秒针umcommon这个）
  
  var time = h + ":" + m + ":" + s + " " + session;
  // （如果要显示秒针用这个）
  // var time = h + ":" + m + " " + session;
  document.getElementById("DigitalCLOCK").innerText = time;
  document.getElementById("DigitalCLOCK").textContent = time;
  
  setTimeout(showTime, 1000);
  
}

showTime();


var $picked = $("#picked"); // 预览挑选的颜色
var canvas = $('#canvas_picker')[0];
var context = canvas.getContext('2d');
var counter =0;


$("#file_upload").change(function (e) {
var F = this.files[0];
var reader = new FileReader();
reader.onload = imageIsLoaded;
reader.readAsDataURL(F);  
});

function imageIsLoaded(e) {
var img = new Image();
img.onload = function(){
  canvas.width  = 500;    // 调整画布大小,相对于图像大小
  canvas.height = 500*this.height/this.width; 
  context.drawImage(this, 0, 0, canvas.width, canvas.height); // 在 0, 0画图, not 10, 10
};
img.src = e.target.result;
}

$('#canvas_picker').click(function(event){
var x = event.pageX - $(this).offset().left; // 相对于canvas offs固定坐标
var y = event.pageY - $(this).offset().top;
var img_data = context.getImageData(x,y , 1, 1).data;
window.img_data = img_data;
var R = img_data[0];
var G = img_data[1];
var B = img_data[2]; 
var rgb = R + ',' + G + ',' + B ;
var hex = rgbToHex(R,G,B);
$('#rgb input').val( rgb );
$('#hex input').val('#' + hex);
if(counter<=3){
$picked.append("<span id='zeshu' style='background:#"+hex+"'>"+rgb+"</span>");
counter++;
}
if(counter>3){
$("span").remove();
counter =0;
}

// testme()
});

function rgbToHex(R, G, B) {
return toHex(R) + toHex(G) + toHex(B);
}

function toHex(n) {
n = parseInt(n, 10);
if (isNaN(n))  return "00";
n = Math.max(0, Math.min(n, 255));
return "0123456789ABCDEF".charAt((n - n % 16) / 16)  + "0123456789ABCDEF".charAt(n % 16);
}