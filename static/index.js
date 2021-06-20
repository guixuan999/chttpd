$(document).ready(function(){
   $("input[name='submit']").click(function(event) {
        let parent_id = $(this).parent().attr("id");
        let src_filename = parent_id.split("#")[0];
        let var_name = parent_id.split("#")[1];

        // for jquery 3.6.0, refer to https://api.jquery.com/
        let var_type = $("span[name='type']", $(this).parent()).text();

        console.log("var_type", var_type);

        // for jquery 3.6.0, refer to https://api.jquery.com/
        let var_node = $("input[name='var']", $("li[id='" + parent_id + "']"));
        // above line is so complicated, actually you can use the following line instead.
        //let var_node = $("input[name='var']", $(this).parent());

        let var_value = null;
        var value_parse_error = false;
        switch(var_type) {
        case "int":
            try {
                var_value = parseInt(var_node.val());  // might be NaN, so we never reach catch clause
            }
            catch(err) {
                value_parse_error = true;
            }
            break;
        case "float":
            try {
                var_value = parseFloat(var_node.val());
            }
            catch(err) {
                value_parse_error = true;
            }
            break;
        case "string":
            var_value = var_node.val();
            break;
        case "bool":
            if(var_node.val() == "true")
                var_value = true;
            else if(var_node.val() == "false")
                var_value = false;
            else
                value_parse_error = true;
            break;
        default:
            value_parse_error = true;
        }

        refresh = function() {
            // refresh
            $.get("/get", "", function(data, textStatus, jqXHR) {
                for(let item of data) {
                    if(item.src_filename == src_filename && item.var_name == var_name) {
                        var_node.val(item.var);
                        break;
                    }
                }
            }).fail(function(xhr, status, info) {
                console.log("get /get fail");
            });
        }

        if(value_parse_error) {
            alert("value parse error! Plese follow the value type.");
            refresh();
            return;
        }

        var data = JSON.stringify([
            {
                src_filename: src_filename,
                var_name: var_name,
                var_type: var_type,
                var: var_value
            }
        ]);

        // for jquery 3.6.0, refer to https://api.jquery.com/
        $.post("/set", data, function(data, textStatus, jqXHR) {
            console.log("post /set success");
            alert("Sucess!");
            
        }).fail(function(xhr, status, info) {
            console.log(xhr, status, info);
            if(xhr.readyState == 4)
                alert(xhr.responseText);
            else
                alert("Error!!!");
        }).always(function(xhr, status, info) {
            refresh();
        });
   });
});
