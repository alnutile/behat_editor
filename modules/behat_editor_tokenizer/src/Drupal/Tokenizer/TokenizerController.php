<?php namespace Drupal\Tokenizer;

use Tokenizer\TokenizerModel;
use Symfony\Component\Filesystem\Filesystem;
use Symfony\Component\Yaml\Yaml;
use Twig_Environment;
use Twig_Loader_Filesystem;

class TokenizerController {

    protected $model;
    public $token_array_preprocess;
    public $token_array_postprocess;
    public $token_string;
    public $template;
    public $loader;
    public $results;

    public function __construct(TokenizerModel $model)
    {
        $this->model = $model;
    }

    public function retrieve()
    {
        $this->results = $this->model->retrieve();
        $this->processFromModelToView();
        return $this->results;
    }

    public function create()
    {
        $this->results = $this->model->create();
        return $this->results;
    }

    public function delete()
    {
        $this->results = $this->model->delete();
        return $this->results;
    }

    public function update()
    {
        $this->results = $this->model->update();
        return $this->results;
    }

    public function index()
    {
        $this->results = $this->model->index();
        return $this->results;
    }

    protected function getTokenArray()
    {
        return $this->token_array_postprocess;
    }

    protected function getTokenString()
    {
        return $this->token_string;
    }

    protected function processFromModelToView()
    {
        $loader = new Twig_Loader_Filesystem(__DIR__ . '/templates/');
        $twig = new Twig_Environment($loader);

        $this->token_array_preprocess = $this->results['content'];
        $token_content_tweak = null;
        $count = 0;
        foreach($this->token_array_preprocess as $key => $value) {
            if(strpos($key, '#') !== FALSE) {
                $token_content_tweak_array["comment|$count"] = $key;
                $count = $count + 1;
            } else {
                $token_content_tweak_array[$key] = $value;
            }
        }

        $this->token_array_postprocess = $twig->render('editable_table.html', array('tokens' => $token_content_tweak_array));
        $this->results['content'] = $this->token_array_postprocess;
    }

    public static function processFromViewToModel($content)
    {
        $token_content_tweak = array();
        foreach($content as $key => $value) {
            //Key of token
            $token_key = $value[0];
            if($token_key != 'new key') {
                //Value of token
                $token_value = $value[1];
                if(strpos($token_key, 'comment') !== FALSE) {
                    $token_key = trim($token_value);
                    $token_value = '';
                }
                $token_content_tweak[trim($token_key)] = trim($token_value);
            }
        }
        return $token_content_tweak;
    }
}