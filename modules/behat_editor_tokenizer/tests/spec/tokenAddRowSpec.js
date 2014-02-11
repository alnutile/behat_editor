describe("Tokenizer", function() {
    var Drupal = {};
    Drupal.behaviors = {};
    Drupal.behaviors.attach = {};
    it("should show a token table if there are no files", function() {
        Drupal.behaviors.behat_editor_tokenizer_app();
        expect('.token-table').toEqual('Test');
    });
});