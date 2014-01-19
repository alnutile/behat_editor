<?php

namespace Drupal\BehatEditorReports;


class BehatServiceReportsModel {
    //Get all results
    // paginate as at 100
    public function get_all() {
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->range(0, 100);
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results = unserialize($record->results);
                $rows[] = (array) $record;
            }
        }
        return array('results' => $rows, 'error' => 0);
    }
}