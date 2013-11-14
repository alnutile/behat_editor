<?php

use Drupal\BehatEditor;

/**
 * @file
 *   Show reports for pages
 */

function behat_editor_report() {
    $arg = arg(3);
    if(!empty($arg)) {
        composer_manager_register_autoloader();
        $rids = array_slice(arg(), 3);
        $build['intro'] = array(
            '#markup' => t('<h3>Below are the results for the following tests <small>@tests</small></h3>', array('@tests' => "#" . implode(' #', $rids)))
        );

        $results = BehatEditor\Results::getResultsForRids($rids);
        if($results['results']) {
            foreach($results['results'] as $key => $value) {
                //If batch test then want to grab the files / tags being used
                if(strpos($value['filename'], '|')) {
                    //Get Row from Batch Report
                    //if tag then show that tags
                    $batch_rid = explode('|', $value['filename']);
                    $batch_row = BehatEditor\ResultsBatch::getResultsByRid($batch_rid[1]);
                    if($batch_row['results']) {
                        $type = $batch_row['results']['method'];
                        $using = theme('item_list', array('items' => unserialize($batch_row['results']['operations'])));
                    }
                    //if files show the files
                    $title = "<h3>This was part of a batch process #{$batch_rid[1]}</h3>";
                    $title .= "<p class=\"lead\">The type was {$type} using {$using}</p>";

                } else {
                    $title = "<h4>Filename: {$value['filename']}</h4>";
                }
                $output = $title;
                $output .= BehatEditor\Results::generateHTMLOutput($value['results']);
                $build[$key] = array(
                    '#markup' => $output,
                );
            }
        }

        return $build;
    } else {
        return drupal_not_found();
    }
}