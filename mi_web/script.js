function post(){
	var text = $(".postElement").val();
	$.post("http://localhost:8080",{text:text},function(data){
		alert("Respuesta:"+data);
	})
	console.log(text);
}	