<?php

namespace Drupal\BehatEditorReports;

class BehatServiceReportsController {
    public $model;

    public function __construct(BehatServiceReportsModel $model) {
        $this->model = $model;
    }

    public function index($parameters = array()) {
        return $this->model->get_all($parameters);
    }
} 