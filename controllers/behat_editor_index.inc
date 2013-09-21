<?php

function behat_editor_index() {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    $build['top_row'] = array(
        '#prefix' => '<div class="alert alert-info">',
        '#markup' => 'Below is a list of files discovered in enabled modules that
                have a behet_features folder inside.',
        '#suffix' => '</div>'
    );

    $modules = _behat_editor_check_for_modules();
    $files_array = _behat_editor_build_array_of_available_files($modules);
    //Turn into a table
    $behat_table =_behat_editor_produce_table_array($files_array);
    $build['behat_table'] = array(
        '#markup' => $behat_table
    );

    return $build;
}