(function ($) {

    Drupal.behat_editor_saucelabs = {};
    Drupal.behat_editor_saucelabs.saucelabs_check = function(tries, starting_job_id) {
        var max_tries = 10;
        tries = typeof tries !== 'undefined' ? tries : 1;
        starting_job_id = typeof starting_job_id !== 'undefined' ? starting_job_id : 0;
        $.ajax(
            {
                url: '/admin/behat/saucelabs/jobs',
                success: function(data) { Drupal.behat_editor_saucelabs.getStatus(data, tries, max_tries, starting_job_id); }
            }
        )};

    Drupal.behat_editor_saucelabs.getStatus = function(data, tries, max_tries, starting_job_id) {
            if(starting_job_id == 0) {
                starting_job_id = data.latest_id;
                Drupal.behat_editor.renderMessageCustom('Connecting to Saucelabs and waiting for job feedback try '+ tries + ' of ' + max_tries, 'info');
                tries++;
                Drupal.behat_editor_saucelabs.saucelabs_check(tries, starting_job_id);
            } else if (tries < max_tries && starting_job_id === data.latest_id) {
                Drupal.behat_editor.renderMessageCustom('Connecting to Saucelabs and waiting for job feedback try '+ tries + ' of ' + max_tries, 'info');
                tries++;
                Drupal.behat_editor_saucelabs.saucelabs_check(tries, starting_job_id);
            } else {
                //See if we are done cause of max count or because we have a repsonse
                if(starting_job_id != data.latest_id) {
                    var id = data.latest_id;
                    //var url = data.latest_job.video_url;
                    var url = '<a href="https://saucelabs.com/tests/'+ id + '" target="_blank">Job is here</a>';
                    //var screenshot = 'https://saucelabs.com/jobs/'+id+'/0000screenshot.png';
                    var status = data.latest_job.status;
                    Drupal.behat_editor.renderMessageCustom('New SauceLabs job info id ' +id+ ' @ ' +url+ '. Status of the job is "' +status+'"');
                } else {
                    Drupal.behat_editor.renderMessageCustom('We have reached the mx tries  '+ tries + ' of ' + max_tries + '<br>' +
                        'SauceLabs has not responded with a new ID. That does not mean the test did not work though. ' +
                        'Please review your account <a href="https://saucelabs.com/account" target="_blank">Dashboard</a>' +
                        ' for SauceLabs.', 'info');
                }
            }
    };

    Drupal.behaviors.behat_editor_saucelabs_run = {

        attach: function (context) {
            $('a.sauce').click(function(e){
                e.preventDefault();
                if(!$(this).hasClass('disabled')) {
                    var method = 'view-mode';
                    if ($('ul.scenario').attr('data-mode')) {
                        method = $('ul.scenario').data('mode');
                    }
                    var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                    var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                    var parameters = {
                        "method": method,
                        "scenario[]": scenario_array
                    };
                    var url = $(this).attr('href');
                    var latestId = '';
                    //Add this first to get previous job id
                    $.getJSON('/admin/behat/saucelabs/jobs', function(data){
                        var latestId = 0;
                        latestId = data.latest_id;
                        //So I do not miss the lastId before starting a job
                        //since it was created before my first setup of the
                        //staring id
                        Drupal.behat_editor_saucelabs.saucelabs_check(1, latestId);
                        $.post(url, parameters, function(data){
                            Drupal.behat_editor.renderMessage(data);
                        }, "json");
                    });
                }
            });
        }
    };


})(jQuery);