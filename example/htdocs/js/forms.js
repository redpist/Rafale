$(function() {
  if (typeof formErrors != 'undefined') {
    $.each(formErrors, function(k, v) {
      $('#'+v.id).addClass('error').after('<span class="error">'+v.error+'</span>')
      console.log("#"+v.id)
    })
  }
})