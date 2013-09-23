(function ($) {
    Drupal.behaviors.behat_editor_edit = {

        attach: function (context) {

            $('ul.tagit').each(function(){
                var inputId = $(this).data('scenario-id');
                console.log(inputId);
                $(this).tagit(
                    {
                        singleField: true,
                        singleFieldNode: $('#scenario-values-'+inputId+''),
                        placeholderText: '@scenario_tag'
                    }
                );
            });

            $('a.run').click(function(e){
                e.preventDefault();
                var method = 'create-mode';
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var url = $(this).attr('href');
                var parameters = {
                    "method": method,
                    "scenario[]": scenario_array
                };
                $.post(url, parameters, function(data){
                    //this console.log forces reload of iframe cache in Chromes
                    console.log(data);
                    Drupal.behat_editor.renderMessage(data);
                }, "json");
            });
        }
    };

})(jQuery);