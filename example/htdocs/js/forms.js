$(function() {
  $.each(formErrors, function(k, v) {
    $('#'+v.id).addClass("error")
    console.log("#"+v.id)
  })
})