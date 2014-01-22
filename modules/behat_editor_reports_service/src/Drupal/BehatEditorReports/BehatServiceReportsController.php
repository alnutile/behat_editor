<?php

namespace Drupal\BehatEditorReports;

class BehatServiceReportsController {
    public $model;

    public function __construct($model = FALSE) {
        //Enable mockery
        if(!$model) {
            $this->model = new BehatServiceReportsModel();
        } else {
            $this->model = $model;
        }
    }

    public function index($parameters = array()) {
        if(empty($parameters)) {
            return $this->model->get_all();
        }
    }
} 