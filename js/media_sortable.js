(function ($, Drupal, window, document, undefined) {

Drupal.behaviors.mediaSortable = {
  attach: function(context, settings) {

    $(function() {

      $('.media-sortable').sortable({
        scroll: false,
        placeholder: 'media-sortable-placeholder',
        forcePlaceholderSize: true,
        start: function(event, ui) {
          ui.placeholder.css({
            border: '1px dashed #ddd'
          });
        },
        update: function(event, ui) {
          var object = ui.item;
          var weight = 0;
          $(ui.item).parent().find('li').each(function() {
            $(this).find('.media-sortable-weight').val(weight);
            weight++;
          });
        }
      });

      $('.media-sortable-trash').droppable({
        over: function(event, ui) {
          $(event.target).css({
            background: '#999',
            color: '#eee'
          });
        },
        out: function(event, ui) {
          $(event.target).css({
            background: '',
            color: ''
          });
        },
        drop: function(event, ui) {
          var submitButton = ui.draggable.find('.remove');
          $(submitButton).mousedown();
        }
      });


      // Weight and form submit don't need to be visible
      $('.media-sortable-weight,.form-submit.remove').hide();

    });

  }
};

})(jQuery, Drupal, this, this.document);
