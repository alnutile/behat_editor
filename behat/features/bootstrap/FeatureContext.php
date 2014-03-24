<?php

use Behat\Behat\Context\ClosuredContextInterface,
    Behat\Behat\Context\TranslatedContextInterface,
    Behat\Behat\Context\BehatContext,
    Behat\Behat\Exception\PendingException;
use Behat\Gherkin\Node\PyStringNode,
    Behat\Gherkin\Node\TableNode;
use Drupal\DrupalExtension;
use Behat\MinkExtension\Context\MinkContext,
    OrangeDigital\BusinessSelectorExtension\Context\BusinessSelectorContext;

//
// Require 3rd-party libraries here:
//
//   require_once 'PHPUnit/Autoload.php';
//   require_once 'PHPUnit/Framework/Assert/Functions.php';
//

/**
 * Features context.
 */
// class FeatureContext extends BehatContext
class FeatureContext extends Drupal\DrupalExtension\Context\DrupalContext
{

    public function __construct(array $parameters)
    {
        //$this->useContext('mink', new MinkContext($parameters));
        $this->useContext('BusinessSelectors', new BusinessSelectorContext($parameters));
    }

    /**
     * See if element is not visible
     *
     * @Then /^element "([^"]*)" is not visible$/
     */
    public function elementIsNotVisible($arg) {

        $el = $this->getSession()->getPage()->find('css', $arg);
        if($el) {
            if($el->isVisible()){
                throw new Exception('Element is visible');
            }
        } else {
            throw new Exception('Element not found');
        }
    }
}
