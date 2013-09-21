(function($) {
    $.fn.closeButton = function() {
        var target = this.parent('li');
        if(target.hasClass('name')) {
            target.prev('li.ignore').remove();
            target.prev('li.tag').remove();
        }
        target.remove();
    };
    return this;
})(jQuery);