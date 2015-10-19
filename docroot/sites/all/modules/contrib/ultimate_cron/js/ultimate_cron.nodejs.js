(function ($) {
  Drupal.Nodejs.callbacks.nodejsUltimateCron = {
    disabled: false,
    runningJobs: {},
    callback: function (message) {
      if (this.disabled) {
        return;
      }
      var action = message.data.action;
      var job = message.data.job;
      var elements = message.data.elements;

      switch (action) {
        case 'lock':
          job.started = new Date().getTime();
          this.runningJobs[job.name] = job;
          break;

        case 'unlock':
          delete(this.runningJobs[job.name]);
          break;

        case 'progress':
          if (!this.runningJobs[job.name]) {
            $("#ctools-export-ui-list-items-reload").click();
            return;
          }
          break;

      }

      for (var key in elements) {
        if (elements.hasOwnProperty(key)) {
          var value = elements[key];
          $(key).replaceWith(value);
          Drupal.attachBehaviors($(key));
        }
      }
    }
  };

  Drupal.behaviors.ultimateCronJobNodejs = {
    attach: function (context) {
      $("tr td.ctools-export-ui-status", context).each(function() {
        var row = $(this).parent('tr');
        var name = $(row).attr('id');
        if ($(this).attr('title') == 'running') {
          var duration = $("tr#" + name + " td.ctools-export-ui-duration span.duration-time").attr('data-src');
          Drupal.Nodejs.callbacks.nodejsUltimateCron.runningJobs[name] = {
            started: (new Date().getTime()) - (duration * 1000),
          };
        }
        else {
          delete(Drupal.Nodejs.callbacks.nodejsUltimateCron.runningJobs[name]);
        }
      });
    }
  };

  setInterval(function() {
    var time = new Date().getTime();
    var jobs = Drupal.Nodejs.callbacks.nodejsUltimateCron.runningJobs;

    for (var name in jobs) {
      if (jobs.hasOwnProperty(name)) {
        var job = jobs[name];
        var date = new Date(time - job.started);
        var minutes = '00' + date.getUTCMinutes();
        var seconds = '00' + date.getUTCSeconds();
        var formatted = minutes.substring(minutes.length - 2) + ':' + seconds.substring(seconds.length - 2);
        $("tr#" + name + " td.ctools-export-ui-duration .duration-time").html(formatted);
      }
    }
  }, 1000)

}(jQuery));

