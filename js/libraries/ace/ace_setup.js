(function ($) {
    $(document).ready(function(){
        Drupal.ace = {};
        Drupal.ace.editor = ace.edit("test-textbox");
        Drupal.ace.editor.setTheme("ace/theme/monokai");
        Drupal.ace.editor.getSession().setMode("ace/mode/text");
    });

})(jQuery);