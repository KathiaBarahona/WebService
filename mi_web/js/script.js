function post() {
    var text = $(".postElement").val();
    $.post("http://localhost:8080", { text: text }, function(data) {
        $('.response').text($('.response').text()+"\n\r"+data);
    });
}

function put() {

    var text = $(".putElement").val();
    $.ajax({
        url: 'http://localhost:8080',
        type: 'PUT',
        data: {text: text},
        success: function(data) {
             $('.response').text($('.response').text()+"\n\r"+data);
        }
    });
    
}
