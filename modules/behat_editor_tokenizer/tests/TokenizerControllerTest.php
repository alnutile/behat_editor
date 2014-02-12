<?php
/**
 * Created by PhpStorm.
 * User: alfrednutile
 * Date: 2/8/14
 * Time: 1:13 PM
 */

namespace Drupal\Tokenizer;

use org\bovigo\vfs\vfsStream;
use org\bovigo\vfs\vfsStreamDirectory;
use org\bovigo\vfs\vfsStreamWrapper;
use Symfony\Component\Filesystem\Filesystem;
use Symfony\Component\Yaml\Yaml;
use \Mockery as m;
use Tokenizer\TokenizerModel;

class TokenizerControllerTest extends \PHPUnit_Framework_TestCase {
    protected $root;
    protected $results;

    public function setUp()
    {
        $this->root = vfsStream::setup('testDir');
    }

    public function tearDown()
    {
        m::close();
    }

    public function testRetrieve()
    {
        $test_results = array('content' => $this->yamlTestArray(), 'errors' => 0, 'filename' => 'wikipedia.12345.token');
        $tokenModel = m::mock('Tokenizer\TokenizerModel');
        $tokenModel->shouldReceive('retrieve')->once()
            ->andReturn($test_results);
        $tokenController = new TokenizerController($tokenModel);
        $results = $tokenController->retrieve();
        $this->assertContains('wikipedia_12345_token', $results['content']);
        $this->assertContains('Default URL', $results['content']);
        $this->assertContains('Key 1 Set 1', $results['content']);
        $this->assertContains('Key 2 Set 1', $results['content']);
        $this->assertContains('Key 3 Set 1', $results['content']);
        $this->assertContains('data-filename="wikipedia.12345.token"', $results['content']);
        $this->assertContains('data-target="wikipedia_12345_token"', $results['content']);
    }

    public function testProcessFromViewToModel()
    {
        $results = TokenizerController::processFromViewToModel($this->yamlTestArray());
        //var_dump($results);
    }

    public function yamlTest()
    {
        $file = <<<HEREDOC
'Default URL': 'Test Secure'
'Key 1 Set 1': 'Token 2'
'Key 2 Set 1': 'Token 4'
'Key 3 Set 1': 'Token 5'
HEREDOC;
        return $file;
    }

    public function yamlTestArray()
    {
        return array(
            'Default URL' => 'Test Secure',
            'Key 1 Set 1' => 'Token 2',
            'Key 2 Set 1' => 'Token 4',
            'Key 3 Set 1' => 'Token 5',
        );
    }

    public function htmlOutput()
    {
        $html = <<<HEREDOC
<table class="tokens table table-striped table-condensed" id="wikipedia_12345_token">
    <thead>
    <tr>
        <th>Key</th>
        <th>Token</th>
    </tr>
    </thead>
    <tr>
        <td><strong>0</strong></td><td><a data-value="Default Url: &#039;Some Domain&#039;
Foo: Bar
Foo2: Bar2" class="selectable" href="#" id="Default Url: &#039;Some Domain&#039;
Foo: Bar
Foo2: Bar2" data-type="select" data-title="Default Url: &#039;Some Domain&#039;
Foo: Bar
Foo2: Bar2" ></a></td>
    </tr>

</table>
<button data-target="wikipedia_12345_token" class="new-row btn btn-sm btn-info">add row</button>
<button data-target="wikipedia_12345_token" data-filename="wikipedia.12345.token"  class="save-row btn btn-sm btn-danger">save this test</button>
<hr>
HEREDOC;
        return $html;

    }
}
 