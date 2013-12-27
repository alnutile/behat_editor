<?php
/**
 * @file
 * To setup custom Step Definitions
 * user the api DrupalExtensions offers
 *
 * @link
 * http://dspeak.com/drupalextension/subcontexts.html
 */

use Drupal\DrupalExtension\Context\DrupalSubContextInterface;
use Behat\Behat\Context\BehatContext;

class BehatEditorSubContext extends BehatContext implements DrupalSubContextInterface {

    public function __construct(){
    }

    /**
     * Setup the needed SubContext alias
     */
    public static function getAlias() {
        return 'behat_editor';
    }

    /**
     * Use a xpath to act on a module
     *
     * @When /^I press the xpath "([^"]*)"$/
     */
    public function iPressTheXpath($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('xpath', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('xpath', $arg)->press();
        } else {
            throw new Exception('Element not found');
        }
    }

    /**
     * Use a xpath to act on a module
     *
     * @When /^I follow the xpath "([^"]*)"$/
     */
    public function iFollowTheXpath($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('xpath', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('xpath', $arg)->click();
        } else {
            throw new Exception('Element not found');
        }
    }

    /**
     * Use a css path to act on a module
     *
     * @When /^I press the element "([^"]*)"$/
     */
    public function iPressTheElement($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('css', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('css', $arg)->press();
        } else {
            throw new Exception('Element not found');
        }
    }

    /**
     * Use a xpath to act on a module
     *
     * @When /^I follow the element "([^"]*)"$/
     */
    public function iFollowTheElement($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('css', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('css', $arg)->click();
        } else {
            throw new Exception('Element not found');
        }
    }
    
    /**
     * Use a css path to act on a module
     *
     * @When /^I check the element "([^"]*)"$/
     */
    public function iCheckTheElement($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('css', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('css', $arg)->check();
        } else {
            throw new Exception('Element not found');
        }
    }
    
    /**
     * Use a xpath to act on a module
     *
     * @When /^I check the xpath "([^"]*)"$/
     */
    public function iCheckTheXpath($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('xpath', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('xpath', $arg)->check();
        } else {
            throw new Exception('Element not found');
        }
    }
    
    /**
     * Use a css path to act on a module
     *
     * @When /^I uncheck the element "([^"]*)"$/
     */
    public function iUncheckTheElement($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('css', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('css', $arg)->uncheck();
        } else {
            throw new Exception('Element not found');
        }
    }
    
    /**
     * Use a xpath to act on a module
     *
     * @When /^I uncheck the xpath "([^"]*)"$/
     */
    public function iUncheckTheXpath($arg)
    {
        $node = $this->getMainContext()->getSession()->getPage()->find('xpath', $arg);
        if($node) {
            $this->getMainContext()->getSession()->getPage()->find('xpath', $arg)->uncheck();
        } else {
            throw new Exception('Element not found');
        }
    }
    
     /**
     * See if Element has style eg p.padL8 has style font-size= 12px
     *
     * @Then /^the xpath "([^"]*)" should have style "([^"]*)"$/
     */
    public function theXpathShouldHaveStyle($arg1, $arg2)
    {

        $element = $this->getMainContext()->getSession()->getPage()->find('css', $arg1);
        if($element) {
            if(strpos($element->getAttribute('style'), $arg2) === FALSE) {
                throw new Exception('Style not found');
            }
        } else {
            throw new Exception('Element not found');
        }
    }
    
    /**
     * Set a waiting time in seconds
     *
     * @Given /^I wait for "([^"]*)" seconds$/
     */
    public function iWaitForSeconds($arg1) {
        sleep($arg1);
    }
    
    /**
     * Test Redirect results
     *
     * @Then /^the url should redirect to "([^"]*)"$/
     */
    public function theUrlShouldRedirectTo($arg1)
    {
        if($this->getMainContext()->getSession()->getCurrentUrl() != $arg1) {
            throw new Exception('You are not on the expected URL');
        }
    }
}