## api documentation

### RESPONSE is ALWAYS excluded from errors. 

# actuator
  # get
    url: /cgi-bin/api.cgi?actuator
    response: {
      actuatorid:
      arduinoid:
      value:
      type:
      arduinovalueid:
      actuatorname:
    }

  # post
  ## You can have extra elements in your json object the api will simply not look at those. Just make sure you pass the required keys.
    url: /cgi-bin/api.cgi?actuator
    data: JSON --> See tables.h for wich elements. --> NO ID Otherwise validation fails.     
    response: {done: true}

  # delete
    url: /cgi-bin/api.cgi?actuator+'id'
    'id': place the id that has to be removed here.
    response: {done: true} 