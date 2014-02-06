(function ($) {

    Drupal.behat_editor_tokenizer = {};
    Drupal.behat_editor_tokenizer.selectable = function () {

    }

    Drupal.behat_editor_tokenizer.add_row = function (context) {
        $('button.new-row', context).on('click', function (e) {
            e.preventDefault();
            var id = new Date().getTime();
            var row =   '<tr>' +
                '<td>' +
                '<strong>Default URL<strong>' +
                '</td>' +
                '<td>' +
                '<a class="selectable select-url" href="#" id="' + id + '2" data-type="select" ">URL</a>' +
                '</td>' +
                '</tr>' +
                '<td>' +
                '<a class="editable" href="#" id="' + id + '3" data-type="text" ">Key</a>' +
                '</td>' +
                '<td>' +
                '<a class="editable" href="#" id="' + id + '4" data-type="textarea" ">Token</a>' +
                '</td>' +
                '</tr>';
            $('table.tokens', context).append(row);
            $('a.editable', context).editable();
            $('a.selectable', context).editable({
                value: 0,
                source: [ Drupal.behat_editor_tokenizer.selectable() ]
            });
        });
    }

    Drupal.behaviors.behat_editor_tokenizer_app = {

        attach: function (context) {

        }
    }

})(jQuery);