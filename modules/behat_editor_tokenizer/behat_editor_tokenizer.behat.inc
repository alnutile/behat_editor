<?php

use Drupal\DrupalExtension\Context\DrupalSubContextInterface;
use Behat\Behat\Context\BehatContext;
use Behat\MinkExtension\Context\MinkContext,
    OrangeDigital\BusinessSelectorExtension\Context\BusinessSelectorContext;

class BehatEditorTokenizerSubContext extends BehatContext implements DrupalSubContextInterface {
    var $originalWindowName = '';
    protected $biz;

    public function __construct(){

    }

    /**
     * @hidden
     *
     * Setup the needed SubContext alias
     */
    public static function getAlias() {
        return 'behat_editor_tokenizer';
    }
}