import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { IMqttServiceOptions, MqttModule } from 'ngx-mqtt';
import { NgxChartsModule } from '@swimlane/ngx-charts';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule } from '@angular/forms';

const MQTT_SERVICE_OPTIONS : IMqttServiceOptions = {
  hostname: '141.145.203.36',
  port: 9001,
  clean: true,
  clientId: 'arduino-mobile-app',
  username: 'ev3',
  password: 'omelette',
};

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    AppRoutingModule,
    NgbModule,
    MqttModule.forRoot(MQTT_SERVICE_OPTIONS),
    NgxChartsModule,
    FormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
