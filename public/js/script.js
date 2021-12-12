firebase.auth().onAuthStateChanged((user)=>{
if(!user){
    location.replace("../index.html")
}else{
    document.getElementById("user").innerHTML = user.email
}
})


function logout(){
    firebase.auth().signOut()
}

/**
 * initApp handles setting up UI event listeners and registering Firebase auth listeners:
 *  - firebase.auth().onAuthStateChanged: This listener is called when the user is signed in or
 *    out, and that is where we update the UI.
 */
// function initApp() {
$(document).ready(function(){
        var ref = firebase.database(); 
        var red = firebase.database().ref('red-led');
        var green = firebase.database().ref('green-led');
        var ldr = firebase.database().ref('ldr');
        var servo = firebase.database().ref('servo');
        //Ultrasonic last 5 value DB reference
        var first = firebase.database().ref('ultrasonic/first');
        var second = firebase.database().ref('ultrasonic/second');
        var third = firebase.database().ref('ultrasonic/third');
        var fourth = firebase.database().ref('ultrasonic/fourth');
        var fifth = firebase.database().ref('ultrasonic/fifth');
        

        var red_check = document.getElementById("red");
        var green_check = document.getElementById("green");
        var slider = document.getElementById("slider");
        var ctx = document.getElementById('myChart').getContext('2d');
        var data={
            labels: [],
            datasets: [{
            label: 'Distance',
            fill: false,
            borderColor: '#2196f3', // Add custom color border (Line)
            backgroundColor: '#2196f3', // Add custom color background (Points and Fill)
            borderWidth: 1 // Specify bar border width
        }]
        }

        var chart = new Chart(ctx, {
            type: 'line',
            data: data,
            options: {
            responsive: true, // Instruct chart js to respond nicely.
            maintainAspectRatio: false, // Add to prevent default behaviour of full-width/height 
            }
        });
        let dp=[];
        let label=[];
        let i=0;
        first.on("value", function (snap) {
        //console.log(snap.val());
        label.push(new Date().toLocaleString().split(',')[1]);
        dp.push(snap.val());
        i = dp.length;
        if(i>5){
        dp.shift();
        label.shift();
        i--;
        }
        data.labels = label;
        data.datasets[0].data=dp;
        console.log(dp);
        chart.update();  
        });

        second.on("value", function (snap) {
        //console.log(snap.val());
        label.push(new Date().toLocaleString().split(',')[1]);
        dp.push(snap.val());
        i=dp.length;
        if(i>5){
        dp.shift();
        label.shift();
        i--;
        }
        data.labels = label;
        data.datasets[0].data=dp;
            chart.update();  
        });
        third.on("value", function (snap) {
        //console.log(snap.val());
        label.push(new Date().toLocaleString().split(',')[1]);
        dp.push(snap.val());
        i = dp.length;
        if(i>5){
        dp.shift();
        label.shift();
        i--;
        }

        data.labels = label;
        data.datasets[0].data=dp;
            chart.update();  
        });
        fourth.on("value", function (snap) {
        //console.log(snap.val());
        label.push(new Date().toLocaleString().split(',')[1]);
        dp.push(snap.val());
        i = dp.length;
        if(i>5){
        dp.shift();
        label.shift();
        i--;
        }

        data.labels = label;
        data.datasets[0].data=dp;
            chart.update();  
        });
        fifth.on("value", function (snap) {
        //console.log(snap.val());
        label.push(new Date().toLocaleString().split(',')[1]);
        dp.push(snap.val());
        i = dp.length;
        if(i>5){
        dp.shift();
        label.shift();
        i--;
        }

        data.labels = label;
        data.datasets[0].data=dp;
            chart.update();  	   
        });
        
        ldr.on("value", function (snap) {
        document.getElementById('canvas').setAttribute("data-value", snap.val());
        });
        servo.on("value", function (snap) {
            slider.value= snap.val();
            document.getElementById('sliderval').innerHTML=snap.val();
        });
        $(function(){                   // Jquery for checking checkbox programitacally 
            red.on("value", function (snap) {
                if(snap.val()==1){
                    //console.log("RED already checked");
                    $('#red').click();
                }
            });
            green.on("value", function (snap) {
                if(snap.val()==1){
                    //console.log("RED already checked");
                    $('#green').click();
                }
            });
            
        });
        red_check.addEventListener('change', function(event) {
        if(event.target.checked){
        console.log("RED checked");
            red.set(1);
        }else{
            red.set(0);
        }
        });
        green_check.addEventListener('change', function(event) {
        if(event.target.checked){
        console.log("GREEN checked");
            green.set(1);
        }else{
            green.set(0);
        }
        });
        slider.addEventListener('change', function(event) {
        document.getElementById("sliderval").innerHTML=event.target.value;
        var pos= parseInt(event.target.value);
        console.log(pos);
        servo.set(pos);
        });
});
