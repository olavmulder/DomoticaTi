## api documentation

# actuator
  # get
    url: /cgi-bin/api.cgi?actuator
    response: {
      id:
      arduinoid:
      value:
      type:
      arduinovalueid:
      actuatorname:
    }

  # post
    url: /cgi-bin/api.cgi?actuator
    data: JSON --> See tables.h for wich elements. --> NO ID Otherwise validation fails.
    response: {done: true}

  # delete
    url: /cgi-bin/api.cgi?actuator+'id'
    'id': place the id that has to be removed here.
    response: {done: true} 