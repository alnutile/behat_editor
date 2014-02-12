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

    /**
     * @When /^I fill in the "([^"]*)" field with "([^"]*)"$/
     */
    public function iFillInTheFieldWith($elementName, $value) {
        $element = $this->findElementWithBusinessSelector($elementName);
        $element->setValue($value);
    }


} 